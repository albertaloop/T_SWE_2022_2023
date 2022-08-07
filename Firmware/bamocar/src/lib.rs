use home::home_dir;
use nix::fcntl::{OFlag};
use nix::pty::{grantpt, posix_openpt, ptsname_r, unlockpt, PtyMaster};
use std::fs;
use std::io::prelude::*;
use std::path::Path;
use std::path::PathBuf;

static CONFIG_PATH: &'static str = ".config/bamocar"; // to prepend with home_dir
static SLAVE_FNAME: &'static str = "slave";

pub fn create_master_slave() -> Result<(PtyMaster, PathBuf), nix::errno::Errno> {
    let master = posix_openpt(OFlag::O_RDWR | OFlag::O_NOCTTY)?;
    grantpt(&master)?;
    unlockpt(&master)?;
    let slave = ptsname_r(&master)?;
    Ok((master, pathbuf_of_string(slave)))
}

fn get_config_path() -> PathBuf {
    Path::new(&home_dir().unwrap()).join(Path::new(&CONFIG_PATH))
}

fn get_slave_path() -> PathBuf {
    get_config_path().join(&SLAVE_FNAME)
}

pub fn pathbuf_of_string(strpath: String) -> PathBuf {
    [strpath].iter().collect()
}

pub fn write_slave_to_config(slave: PathBuf) {
    fs::create_dir_all(&get_config_path()).unwrap();
    fs::write(&get_slave_path(), &slave.as_os_str().to_str().unwrap()).unwrap();  // possible UTF-8 problems here
    log::debug!("updating {:?} with slave {:?}", get_slave_path(), slave);
}

pub fn get_slave_from_config() -> PathBuf {
    let slave = fs::read_to_string(get_slave_path()).unwrap();
    [slave].iter().collect()
}

pub fn buffer_master(fd: &mut nix::pty::PtyMaster, sleep: Option<bool>) {
    let mut buf = [0; 1];
    let response = match fd.read_exact(&mut buf) {
        Err(why) => panic!("couldn't read fd {:?}: {}", fd, why),
        Ok(()) => {
            log::debug!("RX: {:?}", buf[0]);
            bamocar_registers(buf[0])
        },
    };
    match response {
        Err(bad_register) => {
            log::debug!("TX: {:?}", bad_register);
            fd.write_fmt(format_args!("{:?}\n", bad_register)).unwrap()
        },
        Ok(register_value) => {
            log::debug!("TX: {:?}", register_value);
            fd.write_fmt(format_args!("{:?}\n", register_value)).unwrap()
        }
    }
    match sleep.unwrap_or(true) {
        true => std::thread::sleep(std::time::Duration::from_millis(10)),
        false => (),
    }
}

fn bamocar_registers(register: u8) -> Result<u32, String> {
    match register {
        0x05 => Ok(60),
        0x06 => Ok(30),
        0x07 => Ok(100),
        0x08 => Ok(20),
        0x0a => Ok(12),
        0x0b => Ok(231234),
        0x61 => Ok(12345),  // 0x61 is 'a' in ascii
        _    => Err(format!("bamocar register {} does not exist or is not implemented", register)),
    }
}
