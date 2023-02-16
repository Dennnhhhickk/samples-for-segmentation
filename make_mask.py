import cv2
import os
import matplotlib.pyplot as plt

def image_process(path):
    img = cv2.imread(path)
    edges = cv2.Canny(img,2,30,L2gradient=False)
    cv2.imshow('Start image', img)
    plt.imshow(edges, cmap='gray')
    plt.show()
    cv2.waitKey(0)

if __name__ == '__main__':
    with open('images.txt') as file:
        for path in file:
            image_process(path)
