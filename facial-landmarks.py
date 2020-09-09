# import the necessary packages
from imutils import face_utils
import matplotlib.pyplot as plt
import numpy as np
import argparse
import imutils
import dlib
import cv2

p = "models/shape_predictor_68_face_landmarks.dat"
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(p)

# image = cv2.imread('images/profile.jpg')
image = cv2.cvtColor(cv2.imread('images/profile.jpg'), cv2.COLOR_RGB2BGR)
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

rects = detector(gray, 0)
for (i, rect) in enumerate(rects):
    # Make the prediction and transfom it to numpy array
    shape = predictor(gray, rect)
    shape = face_utils.shape_to_np(shape)

    # Draw on our image, all the finded cordinate points (x,y) 
    # for (x, y) in shape:
        # cv2.circle(image, (x, y), 2, (0, 255, 0), -1)

    (x, y, w, h) = face_utils.rect_to_bb(rect)
    cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 3)
        
# cv2.imshow("out",image)
# cv2.waitKey(0)

plt.imshow(image)
plt.show()

# video_capture = cv2.VideoCapture(0)
# flag = 0

# while True:

#     ret, frame = video_capture.read()

#     gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#     rects = detector(gray, 1)

#     for (i, rect) in enumerate(rects):

#         (x, y, w, h) = face_utils.rect_to_bb(rect)

#         cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

#         cv2.imshow('Video', frame)

#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break

# video_capture.release()
# cv2.destroyAllWindows()
