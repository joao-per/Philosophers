<h1 align="center">
	🍝 Philosophers 🍝
</h1>

<p align="center">
	<b><i>Eat, sleep, spaghetti, repeat 🍝</i></b>
</p>

<div align="center">
	
<a href="https://github.com/joao-per/philosophers">![42 Philosopher](https://github.com/joao-per/joao-per/blob/main/Badges/Philosophers.png)</a>

</div>

A simulation of the classic dining philosophers problem. This project is implemented in C and is part of the 42 school curriculum.

### 🎯 Goal
The main objective of this project is to solve the classic dining philosophers problem using threading and mutexes, ensuring no philosopher dies of starvation.

### 🧑‍🔬 Problem Statement
- Multiple philosophers are sitting at a round table with a large bowl of spaghetti in the middle.
- Philosophers can either eat, think, or sleep.
- While eating, they cannot think or sleep. While thinking, they can't eat or sleep. While sleeping, they can't eat or think.
- There are as many forks as there are philosophers on the table.
- To eat, a philosopher needs to pick up the fork on their right and the one on their left.
- After finishing their meal, philosophers put their forks back on the table and start sleeping. Once they wake up, they start thinking.
- The simulation stops if a philosopher dies of starvation.
- Philosophers do not speak with each other and do not know if another philosopher is about to die.

### 💻 How to Run
**Clone the repo:**
```shell
git clone https://github.com/yourusername/PhilosophersDinner.git
```
**Change to the directory:**
```shell
cd PhilosophersDinner
```
**Compile the program:**
```shell
make
```
**Run the program:**	
```shell
./philosophers (philo_nb) (time_to_die) (time_to_eat) (time_to_sleep) [nb_of_eats]
```

### 📜 License
This project is licensed under the MIT License. See the LICENSE file for more details.

### 🙏 Acknowledgements
This project is only for learning purposes. Please don't copy any of the code presented here, try to understand the logic behind it and write your own implementation.

**Happy Coding! 🎉**
