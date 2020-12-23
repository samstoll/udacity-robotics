#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;
    if (!client.call(srv)) {
        ROS_ERROR("Failed to call service drive_bot");
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    uint8_t white_pixel = 255;

    // Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    for (int x = 0; x < img.width; x++) {
        for (int y = 0; y < img.height; y++) {
            // Each row is made up of img.step values, consecutive triplets of rgb values for each of img.width pixels
            uint8_t current_pixel_red = img.data[(y * img.step) + (3 * x)];
            uint8_t current_pixel_green = img.data[(y * img.step) + (3 * x) + 1];
            uint8_t current_pixel_blue = img.data[(y * img.step) + (3 * x)  + 2];

            if (current_pixel_red - white_pixel == 0 &&
                current_pixel_green - white_pixel == 0 &&
                current_pixel_blue - white_pixel == 0) {
                // Found white pixel, drive in its direction
                if (x < (img.width / 3)) {
                    // Left
                    drive_robot(0.0, 0.5);
                } else if (x > ((2 * img.width) / 3)) {
                    // Right
                    drive_robot(0.0, -0.5);
                } else {
                    // Middle
                    drive_robot(0.5, 0.0);
                }
                return;
            }
        }
    }
    // No white pixel found. Stop.
    drive_robot(0.0, 0.0);
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}