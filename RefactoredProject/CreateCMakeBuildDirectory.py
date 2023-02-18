# Copyright 2023 by Georgios Charitos.
# All rights reserved.

from os import chdir
from os import path
from os import system
from sys import exit
from sys import platform
import argparse

class CmdLineArgParser:
    """
    The command line argument parser.
    """
    def is_valid_configuration(self, configuration):
        """
        Check if given configuration is valid.
        :param configuration
        """
        config = str(configuration).lower()
        if config == 'release' or config == 'debug':
            return config[0].upper() + config[1:]
        else:
            raise argparse.ArgumentTypeError(f"Invalid configuration: {configuration}")

    def parse(self):
        """
        Parse command line arguments.
        """
        self.parser = argparse.ArgumentParser(description='Command Line Argument Parser')
        self.parser.add_argument('--configuration', type=self.is_valid_configuration, required=False, default='Release', help='Build configuration i.e. Debug or Release')
        self.parser.add_argument('--compiler', type=str, required=False, default='g++', help='Default compiler in Linux is GCC and in Windows is MSVC. Please specify your desired compiler i.e. Clang')
        self.parser.add_argument('--no-build', dest='build', action='store_false', help='Do not build code after generating build directory')
        self.parser.set_defaults(build=True)
        self.args = self.parser.parse_args()

    def get_config(self):
        """
        Return the build configuration.
        """
        return self.args.configuration

    def get_compiler(self):
        """
        Return the compiler name.
        """
        return self.args.compiler

    def do_build(self):
        """
        Return if build is enabled or not.
        """
        return self.args.build


def main():
    """The main application that parses the given command line arguments and creates the build directory."""
    cmd_line_arg_parser = CmdLineArgParser()
    cmd_line_arg_parser.parse()

    build_directory = 'Build' + cmd_line_arg_parser.get_config()
    build_directory_exists = False

    if path.exists(build_directory):
        build_directory_exists = True
        user_input = str(input(f"The {build_directory} directory already exists. Do you wish to:\n1.Delete it and generate it from scratch\n2.Do nothing\nPlease choose between 1 and 2\n"))

        while (user_input != '1' and user_input != '2'):
            user_input = str(input(f"The {build_directory} directory already exists. Do you wish to:\n1.Delete it and generate it from scratch\n2.Do nothing\nPlease choose between 1 and 2\n"))

        if user_input == '1':
            if platform == 'linux' or platform == 'linux2':
                system(f"rm -rf {build_directory}")
            elif platform == 'win32':
                system(f"rmdir /s /q {build_directory}")

            build_directory_exists = False

        else:
            exit(f"The {build_directory} directory will remain intact.\n")

    if not build_directory_exists:
        system(f"mkdir {build_directory}")

    chdir(f"{build_directory}")

    conan_setup = "conan install --profile default --build=missing ../"

    system(conan_setup)

    cmake_command = f"cmake ../ -DCMAKE_BUILD_TYPE={cmd_line_arg_parser.get_config()}"

    # Code coverage is enabled on Debug configuration on Linux.
    if cmd_line_arg_parser.get_config() == 'Debug' and (platform == 'linux' or platform == 'linux2'):
           cmake_command = cmake_command + ' -DCODE_COVERAGE=ON'

    if cmd_line_arg_parser.get_compiler().lower() == 'clang':
        if platform == 'linux' or platform == 'linux2':
            cmake_command = cmake_command + ' -DCMAKE_CXX_COMPILER=clang++'
        elif platform == 'win32':
            cmake_command = cmake_command + ' -T ClangCL'

    if platform == 'win32':
        cmake_command = cmake_command + ' -A x64'

    # Execute CMake command to generate build directory.
    system(cmake_command)

    # Build code if it is enabled (enabled by default).
    if cmd_line_arg_parser.do_build():
        build_result = system(f"cmake --build . --config {cmd_line_arg_parser.get_config()}")
        if (build_result):
            exit(f"Build failed, error code: {build_result}")


if __name__ == "__main__":
    main()
