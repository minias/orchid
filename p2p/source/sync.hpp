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


#ifndef ORCHID_SYNC_HPP
#define ORCHID_SYNC_HPP

#include <asio/ip/tcp.hpp>
#include <asio/ip/udp.hpp>

#include <cppcoro/async_manual_reset_event.hpp>
#include <cppcoro/async_mutex.hpp>

#include "baton.hpp"
#include "link.hpp"
#include "reader.hpp"
#include "task.hpp"

namespace orc {

template <typename Sync_>
class Sync final :
    public Link
{
  protected:
    Sync_ sync_;

  public:
    template <typename... Args_>
    Sync(BufferDrain *drain, Args_ &&...args) :
        Link(drain),
        sync_(std::forward<Args_>(args)...)
    {
    }

    Sync_ *operator ->() {
        return &sync_;
    }

    size_t Read(Beam &beam) {
        size_t writ;
        try {
            writ = sync_.receive(asio::buffer(beam.data(), beam.size()));
        } catch (const asio::system_error &error) {
            auto code(error.code());
            if (code == asio::error::eof)
                return 0;
            orc_adapt(error);
        }

        if (Verbose)
            Log() << "\e[33mRECV " << writ << " " << beam.subset(0, writ) << "\e[0m" << std::endl;
        return writ;
    }

    void Open() {
        std::thread([this]() {
            Beam beam(2048);
            for (;;) {
                size_t writ;
                try {
                    writ = Read(beam);
                } catch (const Error &error) {
                    orc_insist(!error.text.empty());
                    Link::Stop(error.text);
                    break;
                }

                if (writ == 0) {
                    Link::Stop();
                    break;
                }

                auto subset(beam.subset(0, writ));
                Link::Land(subset);
            }
        }).detach();
    }

    task<void> Shut() override {
        sync_.close();
        co_await Link::Shut();
    }

    task<void> Send(const Buffer &data) override {
        if (Verbose)
            Log() << "\e[35mSEND " << data.size() << " " << data << "\e[0m" << std::endl;

        size_t writ;
        try {
            writ = sync_.send(Sequence(data));
        } catch (const asio::system_error &error) {
            orc_adapt(error);
        }
        orc_assert_(writ == data.size(), "orc_assert(" << writ << " {writ} == " << data.size() << " {data.size()})");

        co_return;
    }
};

}

#endif//ORCHID_SYNC_HPP
