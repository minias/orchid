import {Address} from "./orchid-types";
import Contract from "web3/eth/contract";

export class OrchidContracts {

  static token: Contract;
  static lottery: Contract;

  //static token_addr: Address = '0x53e71f4dec7f0753920a3e039289fdec616ad5b4'; // TST2 Main net
  static token_addr: Address = '0xff9978B7b309021D39a76f52Be377F2B95D72394'; // OTT Main net
  static token_approval_max_gas: number = 50000;

  //static lottery_addr: Address = '0xa38b76b4EbD8f10fFC8866F001bB473874B9ee08'; // Main net
  static lottery_addr: Address = '0x38cf68E1d19a0b2d2Ba73865E4c85aA1A544C1BF'; // Main net

  static lottery_push_max_gas: number = 800000;
  static lottery_push_method_hash: string =
    '0x3cd5941d0d99319105eba5f5393ed93c883f132d251e56819e516005c5e20dbc'; // This is topic[0] of the push event.

  static lottery_pull_amount_max_gas: number = 200000;
  static lottery_pull_all_max_gas: number = 200000;
  static lottery_lock_max_gas: number = 200000;
  static lottery_warn_max_gas: number = 200000;
  static lottery_move_max_gas: number = 200000;

  static token_abi = [
    {
      "inputs": [],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "constructor"
    },
    {
      "anonymous": false,
      "inputs": [
        {
          "indexed": true,
          "internalType": "address",
          "name": "owner",
          "type": "address"
        },
        {
          "indexed": true,
          "internalType": "address",
          "name": "spender",
          "type": "address"
        },
        {
          "indexed": false,
          "internalType": "uint256",
          "name": "value",
          "type": "uint256"
        }
      ],
      "name": "Approval",
      "type": "event"
    },
    {
      "anonymous": false,
      "inputs": [
        {
          "indexed": true,
          "internalType": "address",
          "name": "from",
          "type": "address"
        },
        {
          "indexed": true,
          "internalType": "address",
          "name": "to",
          "type": "address"
        },
        {
          "indexed": false,
          "internalType": "uint256",
          "name": "value",
          "type": "uint256"
        }
      ],
      "name": "Transfer",
      "type": "event"
    },
    {
      "constant": true,
      "inputs": [
        {
          "internalType": "address",
          "name": "owner",
          "type": "address"
        },
        {
          "internalType": "address",
          "name": "spender",
          "type": "address"
        }
      ],
      "name": "allowance",
      "outputs": [
        {
          "internalType": "uint256",
          "name": "",
          "type": "uint256"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "address",
          "name": "spender",
          "type": "address"
        },
        {
          "internalType": "uint256",
          "name": "amount",
          "type": "uint256"
        }
      ],
      "name": "approve",
      "outputs": [
        {
          "internalType": "bool",
          "name": "",
          "type": "bool"
        }
      ],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [
        {
          "internalType": "address",
          "name": "account",
          "type": "address"
        }
      ],
      "name": "balanceOf",
      "outputs": [
        {
          "internalType": "uint256",
          "name": "",
          "type": "uint256"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "decimals",
      "outputs": [
        {
          "internalType": "uint8",
          "name": "",
          "type": "uint8"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "address",
          "name": "spender",
          "type": "address"
        },
        {
          "internalType": "uint256",
          "name": "subtractedValue",
          "type": "uint256"
        }
      ],
      "name": "decreaseAllowance",
      "outputs": [
        {
          "internalType": "bool",
          "name": "",
          "type": "bool"
        }
      ],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "address",
          "name": "spender",
          "type": "address"
        },
        {
          "internalType": "uint256",
          "name": "addedValue",
          "type": "uint256"
        }
      ],
      "name": "increaseAllowance",
      "outputs": [
        {
          "internalType": "bool",
          "name": "",
          "type": "bool"
        }
      ],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "name",
      "outputs": [
        {
          "internalType": "string",
          "name": "",
          "type": "string"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "symbol",
      "outputs": [
        {
          "internalType": "string",
          "name": "",
          "type": "string"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "totalSupply",
      "outputs": [
        {
          "internalType": "uint256",
          "name": "",
          "type": "uint256"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "address",
          "name": "recipient",
          "type": "address"
        },
        {
          "internalType": "uint256",
          "name": "amount",
          "type": "uint256"
        }
      ],
      "name": "transfer",
      "outputs": [
        {
          "internalType": "bool",
          "name": "",
          "type": "bool"
        }
      ],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "address",
          "name": "sender",
          "type": "address"
        },
        {
          "internalType": "address",
          "name": "recipient",
          "type": "address"
        },
        {
          "internalType": "uint256",
          "name": "amount",
          "type": "uint256"
        }
      ],
      "name": "transferFrom",
      "outputs": [
        {
          "internalType": "bool",
          "name": "",
          "type": "bool"
        }
      ],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    }
  ];

  static lottery_abi =
    [
      {
        "inputs": [
          {
            "internalType": "contract IERC20",
            "name": "token",
            "type": "address"
          }
        ],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "constructor"
      },
      {
        "anonymous": false,
        "inputs": [
          {
            "indexed": true,
            "internalType": "address",
            "name": "funder",
            "type": "address"
          },
          {
            "indexed": true,
            "internalType": "address",
            "name": "signer",
            "type": "address"
          },
          {
            "indexed": false,
            "internalType": "uint128",
            "name": "amount",
            "type": "uint128"
          },
          {
            "indexed": false,
            "internalType": "uint128",
            "name": "escrow",
            "type": "uint128"
          },
          {
            "indexed": false,
            "internalType": "uint256",
            "name": "unlock",
            "type": "uint256"
          }
        ],
        "name": "Update",
        "type": "event"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "address",
            "name": "signer",
            "type": "address"
          },
          {
            "internalType": "contract OrchidVerifier",
            "name": "verify",
            "type": "address"
          },
          {
            "internalType": "bytes",
            "name": "shared",
            "type": "bytes"
          }
        ],
        "name": "bind",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "address",
            "name": "funder",
            "type": "address"
          },
          {
            "internalType": "address payable",
            "name": "target",
            "type": "address"
          },
          {
            "internalType": "uint128",
            "name": "amount",
            "type": "uint128"
          },
          {
            "internalType": "bytes",
            "name": "receipt",
            "type": "bytes"
          }
        ],
        "name": "give",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "bytes32",
            "name": "seed",
            "type": "bytes32"
          },
          {
            "internalType": "bytes32",
            "name": "hash",
            "type": "bytes32"
          },
          {
            "internalType": "bytes32",
            "name": "nonce",
            "type": "bytes32"
          },
          {
            "internalType": "uint256",
            "name": "start",
            "type": "uint256"
          },
          {
            "internalType": "uint128",
            "name": "range",
            "type": "uint128"
          },
          {
            "internalType": "uint128",
            "name": "amount",
            "type": "uint128"
          },
          {
            "internalType": "uint128",
            "name": "ratio",
            "type": "uint128"
          },
          {
            "internalType": "address",
            "name": "funder",
            "type": "address"
          },
          {
            "internalType": "address payable",
            "name": "target",
            "type": "address"
          },
          {
            "internalType": "bytes",
            "name": "receipt",
            "type": "bytes"
          },
          {
            "internalType": "uint8",
            "name": "v",
            "type": "uint8"
          },
          {
            "internalType": "bytes32",
            "name": "r",
            "type": "bytes32"
          },
          {
            "internalType": "bytes32",
            "name": "s",
            "type": "bytes32"
          },
          {
            "internalType": "bytes32[]",
            "name": "old",
            "type": "bytes32[]"
          }
        ],
        "name": "grab",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": true,
        "inputs": [
          {
            "internalType": "address",
            "name": "funder",
            "type": "address"
          }
        ],
        "name": "keys",
        "outputs": [
          {
            "internalType": "address[]",
            "name": "",
            "type": "address[]"
          }
        ],
        "payable": false,
        "stateMutability": "view",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "bytes32",
            "name": "ticket",
            "type": "bytes32"
          }
        ],
        "name": "kill",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "address",
            "name": "signer",
            "type": "address"
          }
        ],
        "name": "lock",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": true,
        "inputs": [
          {
            "internalType": "address",
            "name": "funder",
            "type": "address"
          },
          {
            "internalType": "address",
            "name": "signer",
            "type": "address"
          }
        ],
        "name": "look",
        "outputs": [
          {
            "internalType": "uint128",
            "name": "",
            "type": "uint128"
          },
          {
            "internalType": "uint128",
            "name": "",
            "type": "uint128"
          },
          {
            "internalType": "uint256",
            "name": "",
            "type": "uint256"
          },
          {
            "internalType": "contract OrchidVerifier",
            "name": "",
            "type": "address"
          },
          {
            "internalType": "bytes",
            "name": "",
            "type": "bytes"
          }
        ],
        "payable": false,
        "stateMutability": "view",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "address",
            "name": "signer",
            "type": "address"
          },
          {
            "internalType": "uint128",
            "name": "amount",
            "type": "uint128"
          }
        ],
        "name": "move",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": true,
        "inputs": [
          {
            "internalType": "address",
            "name": "funder",
            "type": "address"
          },
          {
            "internalType": "uint256",
            "name": "offset",
            "type": "uint256"
          },
          {
            "internalType": "uint256",
            "name": "count",
            "type": "uint256"
          }
        ],
        "name": "page",
        "outputs": [
          {
            "internalType": "address[]",
            "name": "",
            "type": "address[]"
          }
        ],
        "payable": false,
        "stateMutability": "view",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "address",
            "name": "signer",
            "type": "address"
          },
          {
            "internalType": "address payable",
            "name": "target",
            "type": "address"
          }
        ],
        "name": "pull",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "address",
            "name": "signer",
            "type": "address"
          },
          {
            "internalType": "address payable",
            "name": "target",
            "type": "address"
          },
          {
            "internalType": "uint128",
            "name": "amount",
            "type": "uint128"
          }
        ],
        "name": "pull",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "address",
            "name": "signer",
            "type": "address"
          },
          {
            "internalType": "uint128",
            "name": "total",
            "type": "uint128"
          },
          {
            "internalType": "uint128",
            "name": "escrow",
            "type": "uint128"
          }
        ],
        "name": "push",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": true,
        "inputs": [
          {
            "internalType": "address",
            "name": "funder",
            "type": "address"
          },
          {
            "internalType": "uint256",
            "name": "offset",
            "type": "uint256"
          }
        ],
        "name": "seek",
        "outputs": [
          {
            "internalType": "address",
            "name": "",
            "type": "address"
          }
        ],
        "payable": false,
        "stateMutability": "view",
        "type": "function"
      },
      {
        "constant": true,
        "inputs": [
          {
            "internalType": "address",
            "name": "funder",
            "type": "address"
          }
        ],
        "name": "size",
        "outputs": [
          {
            "internalType": "uint256",
            "name": "",
            "type": "uint256"
          }
        ],
        "payable": false,
        "stateMutability": "view",
        "type": "function"
      },
      {
        "constant": false,
        "inputs": [
          {
            "internalType": "address",
            "name": "signer",
            "type": "address"
          }
        ],
        "name": "warn",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
      },
      {
        "constant": true,
        "inputs": [],
        "name": "what",
        "outputs": [
          {
            "internalType": "contract IERC20",
            "name": "",
            "type": "address"
          }
        ],
        "payable": false,
        "stateMutability": "view",
        "type": "function"
      }
    ];

}

