# Evolution of cooperation under extortion with reinforcement learning

This repository provides the basic C++ simulation code accompanying **“Evolution of cooperation under extortion with reinforcement learning”**.

`basic_TimeSeries.cpp` generates the basic strategy-fraction time-series data underlying **Fig. 4(b–d)**. By changing model parameters and extending the statistical measurements, the same simulation framework can be adapted to investigate other results reported in the manuscript. The supplied program does not generate all figures automatically.

## Model

Two players independently learn to select cooperation (`C`), defection (`D`), or extortion (`E`). Each player maintains a 9 × 3 Q-table. Its state is the previous strategy pair, ordered from its own perspective:

```text
State index:  0   1   2   3   4   5   6   7   8
State:       CC  CD  CE  DC  DD  DE  EC  ED  EE
Action code: C = 0, D = 1, E = 2
```

Both players select actions using an epsilon-greedy policy and update their Q-values synchronously:

```text
Q(s,a) <- (1-alpha) Q(s,a) + alpha [r + gamma max_a' Q(s',a')]
```

Rewards are taken directly from the long-run effective payoff matrix of the selected strategy pair. The program does not explicitly simulate the elementary cooperation/defection rounds within an extortionate strategy. The donation-game normalization is `c = b - 1`, so mutual cooperation yields a payoff of 1. The variable `phi` in the code denotes the manuscript's extortion factor, chi.

## Requirements and compilation

- A C++ compiler.
- A POSIX-compatible environment providing `drand48()` and `srand48()` (for example, Linux or WSL).
- Plotting software to visualize the output. Python with NumPy and Matplotlib is one option.

## Reference

Qingrong Liang, Xin Ou, Jiqiang Zhang, and Li Chen. *Evolution of cooperation under extortion with reinforcement learning*. Accompanying manuscript.

Code repository: https://github.com/chenli-lab/code_Extortion
