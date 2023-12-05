import os
import subprocess

api: bool

src_path     = "."
include_path = ["./include"]
test_src     = "test.c"
except_srcs  = [test_src, ""]

out_name     = "sync"

gcc       = ["gcc"]
gcc_flags = ["-O2", "-Wall"]

gpp       = ["g++"]
gpp_flags = ["-std=c++11", "-O2", "-Wall"]

def find_files(dir: str, ext: str) -> list:
    ret = []

    for f in os.listdir(dir):
        if f.endswith(ext) and not f in except_srcs:
            ret.append(os.path.join(dir, f))

    return ret


def rm_files(files: list):
    for file in files:
        try:
            os.unlink(file)
        except Exception as error:
            print(f"[!] {error}")


def cbuild():
    c_srcs    = find_files(src_path, ".c")
    cpp_srcs  = find_files(src_path, ".cpp")
    includes  = []
    succ      = []

    if not c_srcs:
        print("[!] The target to build does not exist.")
        return
    
    if api is True:
        if not c_srcs:
            print("[!] The target to build does not exist.")
            return
    
    for path in include_path:
        includes += ["-I", path]
    
    for src in c_srcs:
        out  = os.path.basename(src).split(".c")[0] + ".o"
        make = gcc + gcc_flags + includes + ["-c", src, "-o"] + [out]

        try:
            subprocess.check_call(make)
            succ += [out]
        except subprocess.CalledProcessError:
            print("[!] build failed.")
            rm_files(succ)
            return
        
    if api is True:
        for src in cpp_srcs:
            out  = os.path.basename(src).split(".cpp")[0] + ".o"
            make = gpp + gpp_flags + includes + ["-c", src, "-o"] + [out]

            try:
                subprocess.check_call(make)
                succ += [out]
            except subprocess.CalledProcessError:
                print("[!] build failed.")
                rm_files(succ)
                return
    
    try:
        subprocess.run(f"ar rcs lib{out_name}.a %s" % " ".join(succ), check=True, shell=True)
    except subprocess.CalledProcessError:
        print("[!] build failed.")
        rm_files(succ)
        return
    
    rm_files(succ)


def ctest():
    succ     = []
    includes = []

    for path in include_path:
        includes += ["-I", path]
    
    make = gcc + gcc_flags + includes + ["-o", "test", test_src] + ["-L./", "-l%s" % out_name]

    try:
        subprocess.check_call(make)
        succ += ["test"]
    except subprocess.CalledProcessError:
        print("[!] build failed.")
        return
    
    try:
        subprocess.run("./test > test.py; python3 test.py", check=True, shell=True)
        succ += ["test.py"]
    except subprocess.CalledProcessError:
        print("[!] build failed.")
        rm_files(succ)
        return
    
    rm_files(succ)


message_run = '''
 
 1. Build
 2. Build with API (upto: C++11) 
 3. Test Library
 4. Quit

'''

if __name__ == "__main__":
    while (1):
        print(message_run)
        n = int(input("Enter a number: "))
        
        if   n == 1:
            api = False
            cbuild()
        elif n == 2:
            api = True
            cbuild()
        elif n == 3:
            ctest()
        else:
            break
        
