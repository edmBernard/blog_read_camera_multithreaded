"""
Read Camera in separated thread

Usage:
    main.py (IP USER PASS)
    main.py -h | --help
    main.py --version

Options:
    -h --help     Show this screen.
    --version     Show version.
    IP            Ip address of camera
    USER          User
    PASS          Password
"""

import cv2
from docopt import docopt
from threading import Thread


class VideoStream:
    def __init__(self, path):
        self.stream = cv2.VideoCapture(path)
        self.ret, self.frame = self.stream.read()
        self.stopped = False

    def __enter__(self):
        # start a thread to read frames from the camera stream
        t = Thread(target=self.update, args=())
        t.start()
        return self

    def update(self):
        while True:
            # allow to stop thread from main process
            if self.stopped:
                return
            self.ret, self.frame = self.stream.read()

    def read(self):
        return self.frame

    def stop(self):
        self.stopped = True

    def __exit__(self):
        self.stream.release()


def main(ip, user, password):
    with VideoStream("rtsp://{user}:{password}@{ip}:554/video.pro1".format({"ip": ip, "user": user, "password": password})).start() as cap:
        while True:
            frame = cap.read()
            if frame is None:
                continue

            # minimal processing
            frame = cv2.resize(frame, (1280, 720))

            cv2.imshow("Frame", frame)
            key = cv2.waitKey(1)
            if key == 112:
                cap.stop()
                break

    cv2.destroyAllWindows()

if __name__ == '__main__':

    args = docopt(__doc__, version='0.1')
    print(args)

    main(**args)
