#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <sensor_msgs/msg/joint_state.h>

rcl_publisher_t publisher;
sensor_msgs__msg__JointState joint_state_msg;
rclc_support_t support;
rclc_executor_t executor;
rcl_node_t node;
rcl_allocator_t allocator;
rcl_timer_t timer;

#define JOINT_COUNT 6
const char* joint_names[JOINT_COUNT] = {"j1", "j2", "j3", "j4", "j5", "j_finger_left"};

double joint_positions[JOINT_COUNT];
double joint_velocities[JOINT_COUNT];
double joint_efforts[JOINT_COUNT];

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  (void) timer;
  (void) last_call_time;

  for (int i = 0; i < 5; i++) {
    joint_positions[i] = sin(millis()/1000.0 + i*0.5);
    joint_velocities[i] = cos(millis()/1000.0 + i*0.5);
    joint_efforts[i] = 0.0;
  }

  joint_positions[5] = sin(millis()/1000.0) * 0.4;
  joint_velocities[5] = cos(millis()/1000.0) * 0.4;
  joint_efforts[5] = 0.0;

  joint_state_msg.header.stamp.sec = millis()/1000;
  joint_state_msg.header.stamp.nanosec = (millis()%1000)*1000000;
  joint_state_msg.header.frame_id.data = (char*)"";
  joint_state_msg.header.frame_id.size = 0;
  joint_state_msg.header.frame_id.capacity = 0;

  joint_state_msg.name.size = JOINT_COUNT;
  joint_state_msg.name.capacity = JOINT_COUNT;
  joint_state_msg.name.data = (rosidl_runtime_c__String*)malloc(JOINT_COUNT * sizeof(rosidl_runtime_c__String));
  for (int i = 0; i < JOINT_COUNT; i++) {
    joint_state_msg.name.data[i].data = strdup(joint_names[i]);
    joint_state_msg.name.data[i].size = strlen(joint_names[i]);
    joint_state_msg.name.data[i].capacity = strlen(joint_names[i]) + 1;
  }

  joint_state_msg.position.size = JOINT_COUNT;
  joint_state_msg.position.capacity = JOINT_COUNT;
  joint_state_msg.position.data = joint_positions;
  joint_state_msg.velocity.size = JOINT_COUNT;
  joint_state_msg.velocity.capacity = JOINT_COUNT;
  joint_state_msg.velocity.data = joint_velocities;
  joint_state_msg.effort.size = JOINT_COUNT;
  joint_state_msg.effort.capacity = JOINT_COUNT;
  joint_state_msg.effort.data = joint_efforts;

  rcl_publish(&publisher, &joint_state_msg, NULL);

  for (int i = 0; i < JOINT_COUNT; i++) {
    free((void*)joint_state_msg.name.data[i].data);
  }
  free(joint_state_msg.name.data);
}

void error_loop(const char* msg, rcl_ret_t rc) {
  while(1) {
    Serial.print("ERROR: ");
    Serial.print(msg);
    Serial.print(" | return code = ");
    Serial.println((int)rc);
    delay(1000);
    Serial.println("Stuck in error loop - reboot to retry");
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("=== micro-ROS ESP32 starting ===");

  set_microros_serial_transports(Serial);
  delay(1000);

  allocator = rcl_get_default_allocator();

  Serial.println("Initializing rclc_support...");
  rcl_ret_t rc = rclc_support_init(&support, 0, NULL, &allocator);
  if (rc != RCL_RET_OK) {
    error_loop("rclc_support_init failed", rc);
  }
  Serial.println("rclc_support OK");

  Serial.println("Creating node...");
  rc = rclc_node_init_default(&node, "joint_state_publisher", "", &support);
  if (rc != RCL_RET_OK) {
    error_loop("rclc_node_init_default failed", rc);
  }
  Serial.println("Node created OK");

  Serial.println("Creating publisher...");
  rc = rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
    "/joint_states"
  );
  if (rc != RCL_RET_OK) {
    error_loop("rclc_publisher_init_default failed", rc);
  }
  Serial.println("Publisher created OK");

  Serial.println("Creating timer (50ms)...");
  const unsigned int timer_timeout = 50;
  rc = rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback
  );
  if (rc != RCL_RET_OK) {
    error_loop("rclc_timer_init_default failed", rc);
  }
  Serial.println("Timer created OK");

  Serial.println("Creating executor...");
  rc = rclc_executor_init(&executor, &support.context, 1, &allocator);
  if (rc != RCL_RET_OK) {
    error_loop("rclc_executor_init failed", rc);
  }
  rc = rclc_executor_add_timer(&executor, &timer);
  if (rc != RCL_RET_OK) {
    error_loop("rclc_executor_add_timer failed", rc);
  }
  Serial.println("Executor ready");

  Serial.println("=== Setup complete, entering loop ===");
}

void loop() {
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
  delay(10);
  static unsigned long last_print = 0;
  if (millis() - last_print > 1000) {
    Serial.println("Loop alive");
    last_print = millis();
  }
}
