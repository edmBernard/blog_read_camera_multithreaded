"""
Read Camera in separated thread

Usage:
    main.py (IP USER PASSWORD)
    main.py -h | --help
    main.py --version

Options:
    -h --help     Show this screen.
    --version     Show version.
    IP            Ip address of camera
    USER          User
    PASSWORD          Password
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
        while True:  # not self.stopped:
            # allow to stop thread from main process
            if self.stopped:
                return
            self.ret, self.frame = self.stream.read()

    def read(self):
        return self.frame

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.stopped = True
        # self.stream.release()


def main(ip, user, password):
    with VideoStream("rtsp://{user}:{password}@{ip}:554/video.pro1".format(ip=ip, user=user, password=password)) as cap:
        while True:
            frame = cap.read()
            if frame is None:
                continue

            # minimal processing
            frame = cv2.resize(frame, (1280, 720))

            cv2.imshow("Frame", frame)
            key = cv2.waitKey(1)
            if key == 27 or key == 113:
                break

    cv2.destroyAllWindows()

if __name__ == '__main__':

    args = docopt(__doc__, version='0.1')
    print(args)

    main(args["IP"], args["USER"], args["PASSWORD"])
