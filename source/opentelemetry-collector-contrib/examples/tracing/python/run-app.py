#!/usr/bin/env python

import os
import shlex
import subprocess
import sys
import time

def run(cmdline):
    rc = commands.getstatusoutput(cmdline)
    if rc[0]:
        raise Exception("%s failed: %s: %s" % (cmdline, rc[0], rc[1]))
    return rc[1].strip().split("\n")


def run(command, result=False, debug=False, env=os.environ, bg=False):            
    if debug:                                                                   
        print(shlex.split(command))
    process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE,    
                                env=env)                                        
    if bg:
        return
    ret = []                                                                    
    while True:                                                                 
        output = process.stdout.readline()                                      
        if output == '' and process.poll() is not None:                         
            break                                                               

        if output:                                                              
            output = output.strip()                                             
            if result:                                                          
                ret.append(output)                                              
            else:                                                               
                print(sys.stderr, output)                                     

    rc = process.poll()                                                         
    if rc:
        raise Exception("%s failed: %s: %s" % (command, rc, "\n".join(ret)))
    if debug:
    	print("%s:%s" % (command, "\n".join(ret)))
    return ret

run("python3 collector.py 5001", bg=True)
run("python3 collector.py 5002 5001", bg=True)
run("python3 collector.py 5003 5002", bg=True)
run("python3 collector.py 5004 5003", bg=True)

while 1:
    time.sleep(10000)
