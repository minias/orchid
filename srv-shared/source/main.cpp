/* Orchid - WebRTC P2P VPN Market (on Ethereum)
 * Copyright (C) 2017-2019  The Orchid Authors
*/

/* GNU Affero General Public License, Version 3 {{{ */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
/* }}} */


#include <cstdio>
#include <iostream>
#include <mutex>

#include <unistd.h>

#include <boost/filesystem/string_file.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <openssl/base.h>
#include <openssl/pkcs12.h>

#include <pc/webrtc_sdp.h>
#include <rtc_base/message_digest.h>
#include <rtc_base/openssl_identity.h>
#include <rtc_base/ssl_fingerprint.h>

#include "baton.hpp"
#include "cashier.hpp"
#include "channel.hpp"
#include "coinbase.hpp"
#include "egress.hpp"
#include "jsonrpc.hpp"
#include "local.hpp"
#include "node.hpp"
#include "server.hpp"
#include "task.hpp"
#include "trace.hpp"
#include "transport.hpp"
#include "utility.hpp"

namespace bssl {
    BORINGSSL_MAKE_DELETER(PKCS12, PKCS12_free)
    BORINGSSL_MAKE_STACK_DELETER(X509, X509_free)
}

namespace orc {

namespace po = boost::program_options;

std::string Stringify(bssl::UniquePtr<BIO> bio) {
    char *data;
    // BIO_get_mem_data is an inline macro with a char * cast
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-cstyle-cast)
    size_t size(BIO_get_mem_data(bio.get(), &data));
    return {data, size};
}

int Main(int argc, const char *const argv[]) {
    po::variables_map args;

    po::options_description group("general command line");
    group.add_options()
        ("help", "produce help message")
    ;

    po::options_description options;

    { po::options_description group("orchid eth addresses");
    group.add_options()
        //("token", po::value<std::string>()->default_value("0xff9978B7b309021D39a76f52Be377F2B95D72394"))
        ("location", po::value<std::string>()->default_value("0xE214330bDd412F07d8FC4d4960698c0D657e1774"))
        ("lottery", po::value<std::string>()->default_value("0xc999ACfE677239b8F07f04AC378651189c5Ad517"))
    ; options.add(group); }

    { po::options_description group("user eth addresses");
    group.add_options()
        ("personal", po::value<std::string>(), "address to use for making transactions")
        ("password", po::value<std::string>()->default_value(""), "password to unlock personal account")
        ("recipient", po::value<std::string>(), "deposit address for client payments")
        ("provider", po::value<std::string>(), "provider address in stake directory")
    ; options.add(group); }

    { po::options_description group("external resources");
    group.add_options()
        ("rpc", po::value<std::string>()->default_value("http://127.0.0.1:8545/"), "ethereum json/rpc private API endpoint")
        ("stun", po::value<std::string>()->default_value("stun.l.google.com:19302"), "stun server url to use for discovery")
    ; options.add(group); }

    { po::options_description group("webrtc signaling");
    group.add_options()
        ("host", po::value<std::string>(), "external hostname for this server")
        ("bind", po::value<std::string>()->default_value("0.0.0.0"), "ip address for server to bind to")
        ("port", po::value<uint16_t>()->default_value(8443), "port to advertise on blockchain")
        ("path", po::value<std::string>()->default_value("/"), "path of internal https endpoint")
        ("tls", po::value<std::string>(), "tls keys and chain (pkcs#12 encoded)")
        ("dh", po::value<std::string>(), "diffie hellman params (pem encoded)")
    ; options.add(group); }

    { po::options_description group("bandwidth pricing");
    group.add_options()
        ("fiat", po::value<std::string>()->default_value("USD"), "fiat currency for conversions")
        ("price", po::value<std::string>()->default_value("0.03"), "price of bandwidth in fiat / GB")
    ; options.add(group); }

    { po::options_description group("openpvn egress");
    group.add_options()
        ("ovpn-file", po::value<std::string>(), "openvpn .ovpn configuration file")
        ("ovpn-user", po::value<std::string>()->default_value(""), "openvpn client credential (username)")
        ("ovpn-pass", po::value<std::string>()->default_value(""), "openvpn client credential (password)")
    ; options.add(group); }

    po::store(po::parse_command_line(argc, argv, po::options_description()
        .add(group)
        .add(options)
    ), args);

    if (auto path = getenv("ORCHID_CONFIG"))
        po::store(po::parse_config_file(path, po::options_description()
            .add(options)
        ), args);

    po::notify(args);

    if (args.count("help") != 0) {
        std::cout << po::options_description()
            .add(group)
            .add(options)
        << std::endl;

        return 0;
    }


    Initialize();

    std::vector<std::string> ice;
    ice.emplace_back("stun:" + args["stun"].as<std::string>());


    std::string params;

    if (args.count("dh") == 0)
        params =
            "-----BEGIN DH PARAMETERS-----\n"
            "MIIBCAKCAQEA///////////JD9qiIWjCNMTGYouA3BzRKQJOCIpnzHQCC76mOxOb\n"
            "IlFKCHmONATd75UZs806QxswKwpt8l8UN0/hNW1tUcJF5IW1dmJefsb0TELppjft\n"
            "awv/XLb0Brft7jhr+1qJn6WunyQRfEsf5kkoZlHs5Fs9wgB8uKFjvwWY2kg2HFXT\n"
            "mmkWP6j9JM9fg2VdI9yjrZYcYvNWIIVSu57VKQdwlpZtZww1Tkq8mATxdGwIyhgh\n"
            "fDKQXkYuNs474553LBgOhgObJ4Oi7Aeij7XFXfBvTFLJ3ivL9pVYFxg5lUl86pVq\n"
            "5RXSJhiY+gUQFXKOWoqsqmj//////////wIBAg==\n"
            "-----END DH PARAMETERS-----\n"
        ;
    else
        boost::filesystem::load_string_file(args["dh"].as<std::string>(), params);


    std::string key;
    std::string chain;

    if (args.count("tls") == 0) {
        auto pem(Certify()->ToPEM());

        key = pem.private_key();
        chain = pem.certificate();

        // XXX: generate .p12 file (for Nathan)
        std::cerr << key << std::endl;
        std::cerr << chain << std::endl;
    } else {
        bssl::UniquePtr<PKCS12> p12([&]() {
            std::string str;
            boost::filesystem::load_string_file(args["tls"].as<std::string>(), str);

            bssl::UniquePtr<BIO> bio(BIO_new_mem_buf(str.data(), str.size()));
            orc_assert(bio);

            return d2i_PKCS12_bio(bio.get(), nullptr);
        }());

        orc_assert(p12);

        bssl::UniquePtr<EVP_PKEY> pkey;
        bssl::UniquePtr<X509> x509;
        bssl::UniquePtr<STACK_OF(X509)> stack;

        std::tie(pkey, x509, stack) = [&]() {
            EVP_PKEY *pkey(nullptr);
            X509 *x509(nullptr);
            STACK_OF(X509) *stack(nullptr);
            orc_assert(PKCS12_parse(p12.get(), "", &pkey, &x509, &stack));

            return std::tuple<
                bssl::UniquePtr<EVP_PKEY>,
                bssl::UniquePtr<X509>,
                bssl::UniquePtr<STACK_OF(X509)>
            >(pkey, x509, stack);
        }();

        orc_assert(pkey);
        orc_assert(x509);

        key = Stringify([&]() {
            bssl::UniquePtr<BIO> bio(BIO_new(BIO_s_mem()));
            orc_assert(PEM_write_bio_PrivateKey(bio.get(), pkey.get(), nullptr, nullptr, 0, nullptr, nullptr));
            return bio;
        }());

        chain = Stringify([&]() {
            bssl::UniquePtr<BIO> bio(BIO_new(BIO_s_mem()));
            orc_assert(PEM_write_bio_X509(bio.get(), x509.get()));
            return bio;
        }());
    }


    // XXX: the return type of OpenSSLIdentity::FromPEMStrings should be changed :/
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-static-cast-downcast)
    //U<rtc::OpenSSLIdentity> identity(static_cast<rtc::OpenSSLIdentity *>(rtc::OpenSSLIdentity::FromPEMStrings(key, chain));

    rtc::scoped_refptr<rtc::RTCCertificate> certificate(rtc::RTCCertificate::FromPEM(rtc::RTCCertificatePEM(key, chain)));
    U<rtc::SSLFingerprint> fingerprint(rtc::SSLFingerprint::CreateFromCertificate(*certificate));


    std::string host;
    if (args.count("host") != 0)
        host = args["host"].as<std::string>();
    else
        host = boost::asio::ip::host_name();

    auto port(args["port"].as<uint16_t>());
    auto path(args["path"].as<std::string>());

    auto url("https://" + host + ":" + std::to_string(port) + path);
    auto tls(fingerprint->algorithm + " " + fingerprint->GetRfc4572Fingerprint());

    std::cerr << "url = " << url << std::endl;
    std::cerr << "tls = " << tls << std::endl;


    Address location(args["location"].as<std::string>());
    Address personal(args["personal"].as<std::string>());
    std::string password(args["password"].as<std::string>());

    auto rpc(Locator::Parse(args["rpc"].as<std::string>()));
    Endpoint endpoint(GetLocal(), rpc);

    if (args.count("provider") != 0) {
        Address provider(args["provider"].as<std::string>());

        Wait([&]() -> task<void> {
            auto latest(co_await endpoint.Latest());
            static Selector<std::tuple<uint256_t, std::string, std::string, std::string>, Address> look("look");
            if (Slice<1, 4>(co_await look.Call(endpoint, latest, location, provider)) != std::tie(url, tls, "")) {
                static Selector<void, std::string, std::string, std::string> move("move", 3000000);
                co_await move.Send(endpoint, provider, password, location, url, tls, "");
            }
        }());
    }


    // XXX: price needs to be updated at runtime
    // XXX: this code needs to move into Cashier
    uint256_t price(Wait([&]() -> task<uint256_t> {
        cpp_dec_float_50 price(args["price"].as<std::string>());
        price /= co_await Price("ETH", args["fiat"].as<std::string>()) / 200;
        price *= 1000000000;
        price /= 1024 * 1024 * 1024;
        price *= 1000000000;
        using boost::multiprecision::cpp_bin_float_quad;
        co_return static_cast<uint256_t>(static_cast<cpp_bin_float_quad>(price) * static_cast<cpp_bin_float_quad>(uint256_t(1) << 128));
    }()));

    std::cout.precision(std::numeric_limits<cpp_dec_float_50>::digits10);
    std::cerr << "price = " << price << std::endl;

    auto node(Make<Node>(std::move(ice), Make<Cashier>(rpc, Address(args["lottery"].as<std::string>()), price, personal, password)));

    if (args.count("ovpn-file") != 0) {
        std::string ovpnfile;
        boost::filesystem::load_string_file(args["ovpn-file"].as<std::string>(), ovpnfile);

        auto username(args["ovpn-user"].as<std::string>());
        auto password(args["ovpn-pass"].as<std::string>());

        Spawn([&node, ovpnfile = std::move(ovpnfile), username = std::move(username), password = std::move(password)]() -> task<void> {
            auto egress(Make<Sink<Egress>>(0));
            co_await Connect(egress.get(), GetLocal(), 0, ovpnfile, username, password);
            node->Wire() = std::move(egress);
        });
    }


    node->Run(asio::ip::make_address(args["bind"].as<std::string>()), port, path, key, chain, params);
    return 0;
}

}

int main(int argc, const char *const argv[]) { try {
    return orc::Main(argc, argv);
} catch (const std::exception &error) {
    std::cerr << error.what() << std::endl;
    return 1;
} }
