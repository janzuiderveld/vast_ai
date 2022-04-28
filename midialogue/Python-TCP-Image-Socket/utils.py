import glob
import time
import os

# wait for new files to appear in the FTP folder
def wait_new_file(folder):
    """
    Wait for new files to appear in the folder.
    """
    # wait for new files to appear in the FTP folder
    files_old = glob.glob("{}/*".format(folder))
    files_new = []
    # filecount = len(files_old)
    print("Waiting for new files to appear in the FTP folder...")
    while True:
        files_new = glob.glob("{}/*".format(folder))
        filecount = len(files_new)
        if filecount > len(files_old):
            break
        time.sleep(0.01)

    # get new file by unique intersection
    new_file = str(list(set(files_new) - set(files_old))[0])
    print("New file found: " + str(new_file))
    while True:
        if os.path.isfile(str(new_file)): break
        time.sleep(0.01)
    return new_file

