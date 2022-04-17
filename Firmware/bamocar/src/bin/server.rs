use bamocar::*;

fn main() {
    env_logger::init();
    let (mut master_fd, slave) = create_master_slave().unwrap();
    write_slave_to_config(slave);
    loop {
        buffer_master(&mut master_fd, None);
    }
}
