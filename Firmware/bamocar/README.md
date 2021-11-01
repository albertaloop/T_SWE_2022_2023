# Bamocar simulator
Exposes a serial port that mimics the reponses expected from the real Bamocar. This is useful for designing/testing client software.

The Bamocar [NDrive manual](https://www.unitek-industrie-elektronik.de/images/pdf/NDrive/NDrive_EN.pdf) ([uploaded pdf](https://github.com/albertaloop/T_SWE_2019_2020/files/7377322/NDrive_EN.pdf)) is the best reference I have found that breaks down the Bamocar serial communication structure. See section 10.2.

This README is written assuming you are using a POSIX-style OS. This has only been tested on Ubuntu. Other Linux flavors, Mac, Windows should work with minimal tuning.

This project is in alpha-stage, so I'm not going to even list the bugs as it would take me longer to do that than to fix a few of them :). Keep your eye on the Issues tab in GitHub if you want to contribute.

# Setup
This assumes you have [Rust](https://www.rust-lang.org/tools/install) and Python installed. You must start the server before starting a client

Server: open a terminal
```bash
$ cd Firmware/bamocar
$ RUST_LOG=debug cargo run  # runs server by default
$ RUST_LOG=debug cargo run --bin server # or you can directly call the server
```

Rust Client: open a terminal
```bash
$ cd Firmware/bamocar
$ RUST_LOG=debug cargo run --bin client
```

Python Client: open a terminal
```
$ virtualenv --python=python3 venv
$ source venv/bin/activate
$ cd Firmware/bamocar
$ pip install -r src/bin/requirements.txt
$ python3 src/bin/client.py
```

# Developers
If you want to update how the server responds to the client, update the mapping in *<span>lib.rs</span>: bamocar_registers*
