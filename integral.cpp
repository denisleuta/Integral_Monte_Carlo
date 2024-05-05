#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

int monte_carlo_pi(int num_points) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    int points_inside_circle = 0;
    for (int i = 0; i < num_points; ++i) {
        double x = dis(gen);
        double y = dis(gen);
        if (x * x + y * y <= 1.0)
            points_inside_circle++;
    }
    return points_inside_circle;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <num_threads>" << std::endl;
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    int num_points = 10000000; 
    std::vector<std::thread> threads(num_threads);

    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<int> points_inside_circles(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads[i] = std::thread([&points_inside_circles, i, num_points, num_threads]() {
            points_inside_circles[i] = monte_carlo_pi(num_points / num_threads);
        });
    }
  
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    int total_points_inside_circle = 0;
    for (int i = 0; i < num_threads; ++i) {
        total_points_inside_circle += points_inside_circles[i];
    }

    double pi_estimate = 4.0 * total_points_inside_circle / num_points;
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Estimated value of Pi: " << pi_estimate << std::endl;
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}

// При n=10 результаты находились в промежутке между 1.6 и 2.8
// При n=4000 результаты находились в промежутке между 3.091 и 3.158   2мс
// При n=10000 результаты находились в промежутке между 3.1404 и 3.1732   4мс
// При n=50000 реузльтаты находились в промежутке между 3.1328 и 3.14184   11мс
// При n=100000 результаты находились в промежутке между 3.1368 и 3.15104   20мс
// При n=1000000 результаты находились в промежуте между 3.13829 и 3.14338   182мс
// При n=100000000 результаты находились в промежутке между   3.14143 и 3.14158  16123мс
