""" Python script to stage firmware files for building"""
import os
import sys
import subprocess
import glob
import datetime
import shutil
import fnmatch
import random

#Colors for debug output
class ConsoleColors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

#Status for debug
class DebugStatus:
    INFO = ConsoleColors.OKGREEN + "[INFO]"
    FATAL_ERROR = ConsoleColors.FAIL + "[FATAL_ERROR]"
    WARNING = ConsoleColors.WARNING + "[WARNING]"

#Script constants
version_number = 1000 * (random.random())

required_programs = ["protoc", "cog.py"]
project_source_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

nanopb_dir = os.path.join(project_source_dir, "libs/nanopb")
nanopb_compiler = os.path.join(nanopb_dir, "generator/protoc-gen-nanopb")

plugin_dir = os.path.join(project_source_dir, "plugins")

#Helper functions
def get_current_time():
    """
        Returns the current system time

        @rtype: string
        @return: The current time, formatted
    """
    return str(datetime.datetime.now().strftime("%H:%M:%S"))

def print_status(type, message):
    """
        Prints a log message to the console
    """
    debug_status = type + " " + get_current_time() + " " + message + ConsoleColors.ENDC
    print(debug_status)

def lowercase_first_letter(s):
    return s[:1].lower() + s[1:]

def is_program_installed(program):
    """
        Checks to see if program is installed on the linux system

        @type program: string
        @param program: The program name
        @rtype: bool
        @:return: true if the program is installed, false if not
    """
    try:
        output = subprocess.check_output(["which", program])
    except subprocess.CalledProcessError:
            #Subprocess was unable to find the program
            print_status(DebugStatus.FATAL_ERROR, ("%s not found. Install %s to continue!" % (program, program)))
            return False

    #Else, we found the program. Return true
    print_status(DebugStatus.INFO,("Found %s" % program))
    return True

def check_for_required_programs():
    """
        Iterates through the required program list and checks to see if they are installed

        @:return: Aborts the script if a required program is not found
    """
    for program in required_programs:
        if(is_program_installed(program) is False):
            sys.exit("Aborting")


def compile_proto_file(file, generated_dir="./"):
    """
        Compiles the specified proto file using the custom nanopb compiler
        @:return: Aborts the script if error occurs
    """
    print_status(DebugStatus.INFO, ("Compiling proto file: %s" % file))
    
    #Build vars
    #NOTE: Need to go up two levels to get to the nanopb compiler for this MVP script
    protoc_include_path = "--proto_path=" + os.path.join(project_source_dir, "src/proto")
    protoc_plugin_arg = "--plugin=protoc-gen-nanopb=" + nanopb_compiler
    protoc_output_directives = "--nanopb_out=" + generated_dir

    compile_process = subprocess.Popen(["protoc", protoc_include_path, protoc_plugin_arg, protoc_output_directives, file])

    #Wait for this process to finish to continue
    compile_process.communicate()
    print_status(DebugStatus.INFO, "Finished compiling proto file.")

def copytree(src, dst, symlinks=False, ignore=None):
    """
        Credit: http://stackoverflow.com/questions/1868714/how-do-i-copy-an-entire-directory-of-files-into-an-existing-directory-using-pyth
    """
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)

def generate_master_proto(proto_files, parent_dirs):
    """
        Generates Master.proto file
    """
    master_proto_filepath = os.path.join(project_source_dir, "src/common/Master.proto")
    print_status(DebugStatus.INFO, "Generating %s" % master_proto_filepath)
    print_status(DebugStatus.WARNING, "THIS WILL OVERWRITE THE CURRENT Master.proto FILE")

    #Open the file
    with open(master_proto_filepath, "w") as master_proto_file:
        master_proto_file.write("syntax = \"proto2\";\n\n")

        #Add the paths to the plugin protofiles
        for id, proto_file in enumerate(proto_files):
            proto_file_basename = os.path.splitext(proto_file)[0]
            #Set the full path of the protofiles relative to the master_proto_file
            #Hacky hack hack
            # split_path = parent_dirs[id].rpartition('plugins')
            # proto_file_fullpath = os.path.join((split_path[1]+split_path[2]), proto_file)
            master_proto_file.write("import \"%s.proto\";\n" % proto_file_basename)
        #Main master message
        master_proto_file.write("\nmessage MasterMessage\n")
        master_proto_file.write("{\n")
        master_proto_file.write("\trequired sfixed32 type = 1;\n")
        master_proto_file.write("\toneof payload\n")
        master_proto_file.write("\t{\n")
        
        #Plugin payloads
        counter = 2;
        for proto_file in proto_files:
            proto_file_basename = os.path.splitext(proto_file)[0]
            payload_name = lowercase_first_letter(proto_file_basename)
            #Add it to the payload
            print_status(DebugStatus.INFO, "Adding %s to Master.proto payload" % proto_file_basename)
            master_proto_file.write("\t\t%s %s = %d;\n" % (proto_file_basename, payload_name, counter))
            counter = counter + 1
        #End payload
        master_proto_file.write("\t}\n")
        #End master message
        master_proto_file.write("};")
    master_proto_file.close()


def generate_message_types(proto_files):
    """
        Generates MessageTypes.h file
    """
    message_types_file = os.path.join(project_source_dir, "src/common/MessageTypes.h")
    print_status(DebugStatus.INFO, "Generating %s" % message_types_file)
    print_status(DebugStatus.WARNING, "THIS WILL OVERWRITE THE CURRENT MessageTypes.h FILE")
    
    #Open the file
    with open(message_types_file, 'w') as message_file:
        #pragma dec
        message_file.write("#pragma once\n\n")

        #enum declaration
        message_file.write("enum class EMessageType\n")
        message_file.write('{')

        #Default declaration
        message_file.write("\tDefaultMessage,\n")
        #Message types
        for file in proto_files:
            #Strip everything before .proto
            message_def = os.path.splitext(file)[0]
            message_file.write("\t%s,\n" % message_def)
            print_status(DebugStatus.INFO, "Added %s to MessageTypes.h" % message_def)
        #Master message
        message_file.write("\tMasterMessage\n")
        #End enum
        message_file.write("};")    
    message_file.close()

def generate_ncomm_manager():
    """
        This calls cog.py to generate ncomm manager implemenation files
    """
    print_status(DebugStatus.INFO, "Generating NCommManager from template")
    template = os.path.join(project_source_dir, "src/common/NCommManagerTemplate.cpp")
    if not os.path.isfile(template):
        print_status(DebugStatus.FATAL_ERROR, "NO %s FOUND!" % template)
    
    #-d arg is deleting all generated code from outfile
    #-e warn if a file has no cog comments in 
    cog_py_args = "-d"
    outfile = os.path.join(project_source_dir, "src/common/NCommManager.cpp")

    with open(outfile, 'w') as file:
        generate_process = subprocess.Popen(["cog.py", cog_py_args, template], stdout=file)
        #Wait for this process to finish to continue
        generate_process.communicate()
    file.close()
    
    print_status(DebugStatus.INFO, "Finished compiling proto file.")


def stage_source_files():
    """
        Crawls the plugins/ directory for required files for building
        @:return: Aborts the script if error occurs
    """
    #Create the base level plugin dir
    if not os.path.exists(os.path.join(project_source_dir, "src/plugins")):
        os.makedirs(os.path.join(project_source_dir, "src/plugins"))

    proto_files = []
    proto_files_fullpath = []
    #Crawl the plugin directory
    for parent_dir, dirs, files in os.walk(plugin_dir):
        for dir in dirs:
            #Copy the contents of the directory into the source directory for building
            src_plugin_dir = os.path.join(project_source_dir, ("src/plugins/%s") % dir)
            proto_files_fullpath.append(src_plugin_dir)
            if not os.path.exists(src_plugin_dir):
                os.makedirs(src_plugin_dir)
            copytree(os.path.join(plugin_dir,dir), src_plugin_dir)
        
        #Get information about the proto files to generate MessageType.h
        for file in files:
            if file.endswith(".proto"):
                proto_files.append(file)
    
    generate_message_types(proto_files)
    generate_master_proto(proto_files, proto_files_fullpath)
    generate_ncomm_manager()

def generate_proto_file_sources():
    """
        Generates the C++ sources for each proto message
    """
    print_status(DebugStatus.INFO, "Generating C++ code from proto files")
    source_dir = os.path.join(project_source_dir, "src")
    dst_dir = os.path.join(source_dir, "proto")

    #TODO
    #I made the design decision to copy all of the protofiles into one directory
    #And compile them in one swoop
    if not os.path.exists(dst_dir):
        os.makedirs(dst_dir)
    else:
        shutil.rmtree(dst_dir)
        os.makedirs(dst_dir)

    proto_files = []

    for parent_dir, dirs, files in os.walk(source_dir):
        for file in files:
            if file.endswith(".proto") and (parent_dir != dst_dir):
                proto_files.append(os.path.join(parent_dir,file))
            

    #And Copy them over
    for file in proto_files:
        print_status(DebugStatus.INFO, "Copy loop for %s" % file)
        basename = os.path.basename(file)
        if os.path.exists(os.path.join(dst_dir,basename)):
            print_status(DebugStatus.WARNING, "%s already exists. Overwriting" % file)
            os.remove(os.path.join(dst_dir,basename))
        
        if file == dst_dir:
            print_status(DebugStatus.WARNING, "src and dst files are the same. Skipping copy")
        else:
            shutil.copy(file, dst_dir)

    #And compile them
    for proto_file in os.listdir(dst_dir):
        compile_proto_file(os.path.join(dst_dir,proto_file), dst_dir)
def main(argv):
    """Main entry point of the script"""

    #Check if required programs are installed
    check_for_required_programs()

    #Main staging thread
    stage_source_files()

    #Finally, generate proto files
    generate_proto_file_sources()


if __name__ == '__main__':
    sys.exit(main(sys.argv))