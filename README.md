# cerebrum
An attempt at using machine learning to build a technical trading model.

## Idea
Build a reinforcement learning model that is generalizable to any stock as its goal is to be purely quanititative.

## Notes
- Reinforcement learning
  - Agents learns to perform action so as to maximize reward
  - Agent and environment continously interact with each other
  - 3 critical componeents
    - Observation
    - Action
    - Reward
- In order to define a model of this type for stock prediction, we need to quantify the 3 components:
  - Observation: Some vector that contains N samples of percent change and volume.
  - Action: The agent can either buy, sell, or hold.
  - Reward: A specified number of rewards is provided to the model based on the yield the decision produced.
    - Episode ends when 100 actions are completed.

## Sources
- https://www.tensorflow.org/agents
- https://www.tensorflow.org/agents/tutorials/0_intro_rl
- https://www.tensorflow.org/agents/tutorials/2_environments_tutorial

## Run (on Mac)
1. Install dependencies of pygame: https://www.pygame.org/wiki/MacCompile
2. Run "pip install -r requirements.txt"




