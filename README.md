# i3D-SLAM (Beta Version)

Welcome to the **i3D-SLAM Beta Version** repository! This source code contains the beta implementation of the i3D-SLAM system, a real-time, multi-threaded SLAM (Simultaneous Localization and Mapping) solution for 3D perception and navigation. Designed for research and development purposes, i3D-SLAM combines state-of-the-art techniques in depth prediction, local mapping, and loop closure to deliver robust and scalable performance.

## Key Features

- **Real-Time Processing:** Processes images at 8 frames per second for efficient 3D mapping and localization.
- **Multi-Threaded Architecture:** Features dedicated threads for depth prediction, local mapping, sparse bundle adjustment, and loop closure.
- **Parallel Loop Processing:** Includes advanced parallel structures for key-point detection, matching, and motion estimation using RANSAC and Expectation-Maximization (EM) techniques.
- **Dynamic Thread Allocation:** Optimizes computational resource usage to adapt to hardware capabilities.
- **Depth Prediction Integration:** Supports monocular depth prediction using unsupervised learning, leveraging recent advancements in deep learning.
- **Open Platform:** Built with C++ and GTK, compatible with Linux-based environments.

## Purpose

The i3D-SLAM Beta Version is aimed at researchers and developers interested in advancing the field of 3D SLAM. This beta release integrates findings from cutting-edge SLAM research and serves as a platform for further experimentation and enhancement.

## Usage

This repository is provided for academic and research purposes. As a beta release, it may still contain bugs or incomplete features. Feedback, contributions, and suggestions are highly welcome to help refine and improve the system.

## Disclaimer

This is a beta version and is under active development. Use it at your own risk. For any issues or feature requests, please open an issue in the repository.

---

We hope you find i3D-SLAM useful and look forward to your contributions and feedback!

 ## Settings
* Predicted depth monocular SLAM
* Stereo SLAM 

 ![Alt Text](i3DSLAM_beta/samples/i3DSLAMv1.0.gif)
