from os import chdir, path, system, getcwd
from sys import exit, platform
import argparse

class CmdLineArgParser:
    """
    The command line argument parser.
    """
    def is_valid_configuration(self, configuration: str):
        """
        Check if given configuration is valid.
        :param configuration
        """
        valid_configuration = ['release','debug']
        input_configuration = configuration.lower()
        if input_configuration in valid_configuration:
            return input_configuration[0].upper() + input_configuration[1:]
        else:
            raise argparse.ArgumentTypeError(f"Invalid configuration: {configuration}")

    def is_valid_compiler(self, compiler: str):
        """
        Check if given compiler is valid.
        :param compiler
        """
        valid_compiler = ['msvc','gcc','g++','gnu','clang']
        input_compiler = compiler.lower()
        if input_compiler in valid_compiler:
            return input_compiler
        else:
            raise argparse.ArgumentTypeError(f"Invalid compiler: {compiler}")

    def parse(self):
        """
        Parse command line arguments.
        """
        self.parser = argparse.ArgumentParser(description='Command Line Argument Parser')
        self.parser.add_argument('--configuration', type=self.is_valid_configuration, required=False, default='Release', help='Build configuration i.e. Debug or Release')
        self.parser.add_argument('--compiler', type=self.is_valid_compiler, required=False, help='Default compiler on Linux is GCC and on Windows is MSVC. Please specify your desired compiler i.e. Clang')
        if platform == 'win32':
            self.parser.set_defaults(compiler='msvc')
        else:
            self.parser.set_defaults(compiler='g++')
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


class BuildDirectoryManager:
    """
    The build directory manager.
    """
    def __init__(self, config: str, current_dir: str = getcwd()):
        self.build_dir = path.join(current_dir, 'Build' + config)
        if path.exists(self.build_dir) and path.isdir(self.build_dir):
            self.build_dir_exists = True
        else:
            self.build_dir_exists = False


class BuildConanManager:
    """
    The build conan manager.
    """
    def __init__(self, config: str, compiler: str, profile_default: bool = True):
        self.config = config
        self.compiler = compiler
        self.profile_default = profile_default
        self.settings = {'arch':'x86_64'}
        if platform == 'linux':
            self.settings['os'] = 'Linux'
            self.settings['compiler.libcxx'] = 'libstdc++11'
        else:
            self.settings['os'] = 'Windows'
        self.output_folder = '.'

    def install(self):
        conan_install = 'conan install'
        for key,value in self.settings.items():
            conan_install += f" -s {key}={value}"
        conan_install += f" --output-folder={self.output_folder}"
        conan_install += ' -pr:h default -pr:b default'
        conan_install += f" --build=missing -s:b build_type={self.config} -s:h build_type={self.config} ../"
        return conan_install


def get_user_input(build_dir: str):
    """Get user wish given an existing build directory."""
    input_str = f"The {build_dir} directory already exists. Do you wish to:\n1.Delete it and generate it from scratch\n2.Do nothing\nPlease choose between 1 and 2\n"
    user_input = str(input(input_str))

    while (user_input != '1' and user_input != '2'):
        user_input = str(input(input_str))

    if user_input == '1':
        if platform == 'linux':
            system(f"rm -rf {build_dir}")
        elif platform == 'win32':
            system(f"rmdir /s /q {build_dir}")
    else:
        exit(f"The directory: {build_dir} will remain intact.\n")


def main():
    """The main application that parses the given command line arguments and creates the build directory."""
    cmd_line_arg_parser = CmdLineArgParser()
    cmd_line_arg_parser.parse()

    compiler = cmd_line_arg_parser.get_compiler().lower()
    if compiler == 'clang' and platform == 'win32':
        exit(f"clang is not supported on Windows")
    print(f"Build compiler is {compiler}")

    config = cmd_line_arg_parser.get_config()
    print(f"Build configuration is {config}")

    build_dir_manager = BuildDirectoryManager(config)
    build_dir = build_dir_manager.build_dir
    build_dir_exists = build_dir_manager.build_dir_exists

    if build_dir_exists:
        get_user_input(build_dir)

    system(f"mkdir {build_dir}")

    chdir(f"{build_dir}")

    build_conan_manager = BuildConanManager(config, compiler)

    system(build_conan_manager.install())

    cmake_command = f"cmake ../ -DCMAKE_BUILD_TYPE={config}"

    # Code coverage is enabled on Debug configuration on Linux.
    if config == 'Debug' and platform == 'linux':
        cmake_command = cmake_command + ' -DCODE_COVERAGE=ON'

    if compiler == 'clang':
        cmake_command = cmake_command + ' -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang'

    if platform == 'win32':
        cmake_command = cmake_command + ' -A x64'

    # Execute CMake command to generate build directory.
    system(cmake_command)

    # Optionally build code (enabled by default).
    if cmd_line_arg_parser.do_build():
        build_result = system(f"cmake --build . --config {config}")
        if (build_result):
            exit(f"Build failed, error code: {build_result}")


if __name__ == "__main__":
    main()
