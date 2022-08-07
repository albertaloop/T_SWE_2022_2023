import command_module

command_manager = command_module.CommandManager("192.168.0.1", 3000)
command_manager.start()
command_manager.stop()
