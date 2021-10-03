// extern crate serial;
extern crate nix;

use std::env;
use std::path::Path;
use nix::fcntl::{OFlag, open};
use nix::pty::{grantpt, posix_openpt, ptsname_r, unlockpt};
use nix::sys::stat::Mode;
use std::io::prelude::*;



fn main() {
    // let args: Vec<String> = env::args().collect();
    // let _port = &args[1];
    let mut master_fd = posix_openpt(OFlag::O_RDWR | OFlag::O_NOCTTY).unwrap();
    let slave_name = get_slave(&master_fd).unwrap();
    println!("{}", slave_name);
    buffer_master(&mut master_fd);

}

fn get_slave(fd: &nix::pty::PtyMaster) -> Result<String, nix::errno::Errno> {
    match grantpt(&fd) {
        Err(_) => panic!("Grant PT failed"),
        Ok(()) => (),
    };
    match unlockpt(&fd) {
        Err(_) => panic!("Unlock PT failed"),
        Ok(()) => (),
    };
    ptsname_r(&fd)
}

fn buffer_master(fd: &mut nix::pty::PtyMaster) {
    let mut buf = [0; 1];
    match fd.read_exact(&mut buf) {
        Err(why) => panic!("couldn't read fd: {}", why),
        Ok(_) => println!("buf: {:?}", buf),
    };
}

// fn client<T: SerialPort>(port: &mut T) -> std::io::Result<()> {
//     let _slave_fd = open(Path::new(&slave_name), OFlag::O_RDWR, Mode::empty()).unwrap();


//     port.set_timeout(std::time::Duration::from_millis(1000))?;
//     let mut buf: Vec<u8> = (0..255).collect();
//     port.write(&buf[..])?;
//     port.read(&mut buf[..])?;
//     Ok(())
// }