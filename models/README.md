# Checkpoints

[`dqn-policy.zip`](dqn-policy.zip) contains the best DQN checkpoint selected during the original 100,000-step simulation run. Stable-Baselines3 2.1.0 saved it at step 43,000. The requirements use Stable-Baselines3 2.7.0 for Python 3.12 and CPU inference. A warning about the older optimizer state can appear when loading the checkpoint.

[`best.pt`](best.pt) contains the fine-tuned YOLOv8n checkpoint from the final submission. See the [checkpoint license](../licenses/AGPL-3.0.txt) and [training notebook](../notebooks/02-yolov8-detector.ipynb).
