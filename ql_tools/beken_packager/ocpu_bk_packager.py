#!/usr/bin/env python

import os
import sys
import json
import shutil
import argparse
import time

project_name=sys.argv[1]

out_json_2M = {
    "magic": "RT-Thread",
    "version": "0.1",
    "count": 2,
    "section": [
        {
            "firmware": "bootloader_bk7231n_uart2_v1.0.6.bin",
            "version": "2M.1220",
            "partition": "bootloader",
            "start_addr": "0x00000000",
            "size": "65280"
        },
        {
            "firmware": "../../ql_out/"+project_name+".bin",
            "version": "2M.1220",
            "partition": "app",
            "start_addr": "0x00011000",
            "size": "1156K"
        }
    ]
}

full_image_name = "all_2M.1220.bin"
#uart_image_name = project_name+"_uart_2M.1220.bin"

bootloader_str = "bootloader_bk7231n_uart2_v1.0.6.bin"
firmware_str = "../../ql_out/"+project_name+".bin"

out_path = "tmp.json"

out_json_2M["section"][0]["firmware"] = bootloader_str
out_json_2M["section"][1]["firmware"] = firmware_str
out_json_2M = json.dumps(out_json_2M, sort_keys=True, indent=4)

print(out_json_2M)
with open(str(out_path), "w") as f:
    f.write(out_json_2M)

os.system(os.getcwd()+"/beken_packager {}".format(out_path))


shutil.move((os.path.join(os.getcwd(), full_image_name)), "../../ql_out/" + full_image_name)
#shutil.move((os.path.join(os.getcwd(), uart_image_name)), "../../ql_out/" + uart_image_name)
shutil.copy((os.path.join(os.getcwd(), bootloader_str)), "../../ql_out/" + bootloader_str)
time.sleep(1)
os.remove(out_path)
