This README is written assuming you are using a POSIX-style OS. This has only been tested on Ubuntu. Other Linux flavors, Mac, Windows should work with minimal tuning.

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