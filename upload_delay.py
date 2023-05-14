Import("env")


# The tests seem to execute too fast for the Teensy40
# Let's increase the delay to 5 seconds.
# See @https://github.com/platformio/platformio-core/issues/3742

def after_upload(source, target, env):
    print("Delay while uploading...")
    import time
    time.sleep(5)
    print("Done!")


env.AddPostAction("upload", after_upload)
