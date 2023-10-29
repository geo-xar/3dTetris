from os import chdir, path, getcwd, makedirs
from sys import exit, platform
import argparse
import subprocess
import shutil


def is_valid_configuration(configuration: str):
    """
    Check if given configuration is valid.
    :param configuration
    """
    valid_configuration = ['release', 'debug']
    input_configuration = configuration.lower()
    if input_configuration in valid_configuration:
        return input_configuration[0].upper() + input_configuration[1:]
    else:
        raise argparse.ArgumentTypeError(f"Invalid configuration: {configuration}")


def is_valid_compiler(compiler: str):
    """
    Check if given compiler is valid.
    :param compiler
    """
    valid_compiler = ['msvc', 'gcc', 'clang']
    input_compiler = compiler.lower()
    if input_compiler in valid_compiler:
        return input_compiler
    else:
        raise argparse.ArgumentTypeError(f"Invalid compiler: {compiler}")


class CmdLineArgParser:
    """
    The command line argument parser.
    """

    def __init__(self):
        self.args = None
        self.parser = None

    def parse(self):
        """
        Parse command line arguments.
        """
        self.parser = argparse.ArgumentParser(description='Command Line Argument Parser')
        self.parser.add_argument('--configuration', type=is_valid_configuration, required=False, default='Release',
                                 help='Build configuration i.e. Debug or Release')
        self.parser.add_argument('--compiler', type=is_valid_compiler, required=False,
                                 help='Default compiler on Linux is GCC and on Windows is MSVC. Please specify your '
                                      'desired compiler i.e. Clang')
        if platform == 'win32':
            self.parser.set_defaults(compiler='msvc')
        else:
            self.parser.set_defaults(compiler='gcc')
        self.parser.add_argument('--with-build', dest='build', help='Build code after generating build directory')
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


class DirectoryManager:
    """
    The build directory manager.
    """

    def __init__(self, config: str, current_dir: str = getcwd()):
        self.build_dir = path.join(current_dir, 'Build' + config)
        if path.exists(self.build_dir) and path.isdir(self.build_dir):
            self.build_dir_exists = True
        else:
            self.build_dir_exists = False


class ConanManager:
    """
    The conan manager.
    """

    def __init__(self, config: str, compiler: str, profile_default: bool = True):
        self.config = config
        self.compiler = compiler
        self.output_folder = '.'
        self.settings = {'arch': 'x86_64', 'build_type': f"{config}", 'compiler': f"{compiler}",
                         'compiler.cppstd': '20'}

        if platform == 'linux':
            self.settings['os'] = 'Linux'
            self.settings['compiler.libcxx'] = 'libstdc++11'
        elif platform == 'win32':
            self.settings['os'] = 'Windows'
            self.settings['compiler.runtime'] = 'dynamic'

        if compiler == 'msvc':
            self._get_msvc_config()
        elif compiler == 'clang':
            self.settings['compiler.version'] = '16'

        if profile_default:
            subprocess.run("conan profile detect --force", check=True, shell=True)

    def _get_msvc_config(self):
        self.settings['compiler.version'] = '193'

    def install(self):
        conan_install = 'conan install'
        for key, value in self.settings.items():
            conan_install += f" -s:b {key}={value} -s:h {key}={value}"
        if self.compiler == 'clang':
            conan_install += " --profile=../clang-profile.txt"
        conan_install += f" --output-folder={self.output_folder}"
        conan_install += f" --build=missing ../"
        return conan_install


class CmakeManager:
    """
    The cmake manager.
    """

    def __init__(self, config: str, compiler: str):
        self.config = config
        self.compiler = compiler

    def configure(self):
        cmake_command = f"cmake ../ -DCMAKE_BUILD_TYPE={self.config}"

        # Code coverage is enabled on Debug configuration on Linux.
        if self.config == 'Debug' and platform == 'linux':
            cmake_command += ' -DCODE_COVERAGE=ON'

        if platform == 'win32':
            cmake_command += ' -A x64'

        if self.compiler == 'clang':
            cmake_command += ' -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++'

        return cmake_command

    def build(self):
        cmake_command = f"cmake --build . --config {self.config}"
        return cmake_command


def get_user_input(build_dir: str):
    """Get user wish given an existing build directory."""
    input_str = (f"The {build_dir} directory already exists. Do you wish to:\n1.Delete it and generate it from "
                 f"scratch\n2.Do nothing\nPlease choose between 1 and 2\n")
    user_input = str(input(input_str))

    while user_input != '1' and user_input != '2':
        user_input = str(input(input_str))

    if user_input == '1':
        shutil.rmtree(build_dir)
    else:
        exit(f"The directory: {build_dir} will remain intact.\n")


def main():
    """The main application that parses the given command line arguments and creates the build directory."""
    cmd_line_arg_parser = CmdLineArgParser()
    cmd_line_arg_parser.parse()

    compiler = cmd_line_arg_parser.get_compiler().lower()
    print(f"Build compiler is {compiler}")

    config = cmd_line_arg_parser.get_config()
    print(f"Build configuration is {config}")

    dir_manager = DirectoryManager(config)
    build_dir = dir_manager.build_dir
    build_dir_exists = dir_manager.build_dir_exists

    if build_dir_exists:
        get_user_input(build_dir)

    makedirs(build_dir)
    chdir(f"{build_dir}")

    conan_manager = ConanManager(config, compiler)
    subprocess.call(conan_manager.install(), shell=True)

    cmake_manager = CmakeManager(config, compiler)
    subprocess.run(cmake_manager.configure(), check=True, shell=True)

    if cmd_line_arg_parser.do_build():
        subprocess.run(cmake_manager.build(), check=True, shell=True)


if __name__ == "__main__":
    main()
