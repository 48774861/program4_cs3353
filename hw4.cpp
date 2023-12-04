#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;

float overloaded(const float& x) {
    return std::min(x + (float)0.2, std::max((float)0.999, x));
}

pair< vector<int>, float >  assign(int N, int E, int M, float **prob)
{
    vector <int> res0;
    float p = 1;

    if (E < N) {
        throw std::invalid_argument("The rescue fails because there is not enough energy to supply at least 1 energy to each vehicle!");
    }

    E = std::min(E, M*N); // Any energy beyond the value of M*N will not lead to any greater chance of success.

    // Base Cases have been checked.
    if (N == 1) {
        res0.push_back(E);
        return make_pair(res0, prob[0][E - 1]);
    } else if (N == E) {
        for(int k = 0; k < N; k++) {
            p *= prob[k][0]; // Gets the probability of giving 0+1 energy to the kth vehicle.
        }
        for (int i =0; i < N; i++)
            res0.push_back(1); // Give 1 energy to each vehicle.
        
        return make_pair(res0, p);
    }

    // Storage Needed
    float PROB[N][E]; // Rows are the Vehicle Numbers and the Columns are the maximum amount of energy for all vehicles.
    float choice[N][E]; // Keeps how much energy will be given to a vehicle given a certain amount of energy.

    // Initialize Base Case where the amount of energy is equal to the number of vehicles.
    int size_diagonal = std::min(N, E);
    float current_probability = 1;
    for (int y = 0; y < size_diagonal; y++) {
        // Final Probability is the probability of giving 1 energy to each vehicle (that is included at each array element).
        current_probability *= prob[y][0];
        PROB[y][y] = current_probability;
        choice[y][y] = 1;
    }

    // Initialize Base Case where there is only one vehicle.
    for (int y = 0; y < M; y++) {
        // Final Probability is giving the energy at the current array element to that one vehicle.
        PROB[0][y] = prob[0][y];
        choice[0][y] = y+1;
    }
    for (int y = M; y < E; y++) {
        // For scenarios where there is more energy than the one vehicle can take.
        PROB[0][y] = prob[0][M - 1];
        choice[0][y] = M;
    }

    // Iteration
    float currentValue = -1;
    // Calculating the PROBability of success for vehicles 0..p given q + 1 energy.
    for (int p = 1; p < N; p++) {
        for (int q = p + 1; q < E; q++) {

            PROB[p][q] = -1;
            /**
             * q + 1 - p is the loop end because p needs to be <= q as each vehicle needs at least 1 energy.
             * M is the loop end because prob[p][k > M - 1] is equal to prob[p][M-1] and the PROBability of the other vehicles will be lower.
             *  Assuming that decreasing energy always decreases the likelihood of success.
            */
            for (int k = 0; k < min(M, q + 1 - p); k++) {
                
                // q - k is the energy we have left, so q - k - 1 is the index of that energy.
                currentValue = PROB[p - 1][q - k - 1] * prob[p][k];
                if(currentValue > PROB[p][q]) {
                    PROB[p][q] = currentValue;
                    choice[p][q] = k + 1;
                }
            }
        }
    }

    p = PROB[N-1][E-1];

    // Get all choices of given energy to vehicles.
    int total_energy = E;
    int current_vehicle = N-1;
    while (total_energy > 0) {
        res0.push_back(choice[current_vehicle][total_energy - 1]);
        total_energy -= choice[current_vehicle][total_energy - 1];
        current_vehicle--;
    }
    std::reverse(res0.begin(), res0.end());

    return make_pair(res0, p);

}


pair< vector<int>, float >  assign2(int N, int E, int M, float **prob)
{
    vector <int> res0;
    float p = 1;

    if (E < N) {
        throw std::invalid_argument("The rescue fails because there is not enough energy to supply at least 1 energy to each vehicle!");
    }

    // Base Cases have been checked.
    if (N == 1) { // Assume that we have 2 overloads at the start.
        if (E == 1) { // Only 1 energy, so no overloads possible.
            res0.push_back(1);
            return make_pair(res0, prob[0][0]);
        }
        if (E >= M + 1) { // Apply the maximum possible energy to the vehicle and overload it.
            res0.push_back(-M); //
            return make_pair(res0, overloaded(prob[0][M - 1]));
        }
        
        E = std::min(E, M); // Exact energy needed for no overloads.
        float overloaded_p = overloaded(prob[0][E - 2]); // We overload but with 1 less energy.
        if(prob[0][E - 1] > overloaded_p) { // No overload is better.
            res0.push_back(E);
            p = prob[0][E - 1];
        } else { // Overload with 1 less energy is better.
            res0.push_back(-(E - 1));
            p = overloaded_p;
        }
        
        return make_pair(res0, p);
    } else if (N == E) { // Must have no overloads because there is no energy to overload.
        for(int k = 0; k < N; k++) {
            p *= prob[k][0]; // Gets the probability of giving 0+1 energy to the kth vehicle.
        }
        for (int i =0; i < N; i++)
            res0.push_back(1); // Give 1 energy to each vehicle.
        
        return make_pair(res0, p);
    }

    E = std::min(E, M*N + 2); // + 2 are for the extra overloads.

    // Storage Needed
    float PROB[N][E][3]; // 3 is for the number of overloads 0, 1, or 2.
    float choice[N][E][3];

    for(int k = 0; k < 3; k++) {
        for (int p = 0; p < N; p++) {
            for (int q = 0; q < E; q++) {
                PROB[p][q][k] = 0; // Set them all to 0 to avoid errors?
                choice[p][q][k] = 0;
            }
        }
    }

    // Initialize Base Case where the amount of energy is equal to the number of vehicles (so no overloads possible).
    int size_diagonal = std::min(N, E);
    float current_probability = prob[0][0];
    
    for (int y = 1; y < size_diagonal; y++) {
        current_probability *= prob[y][0];
        PROB[y][y][0] = current_probability;
        choice[y][y][0] = 1;
    }

    // Initialize Base Case where there is only one vehicle (so only one overload is possible).
    for(int y = 0; y < 3; y++) { // Energy and Number of Vehicles are Equal to 1, so there is no extra energy for any overloads.
        PROB[0][0][y] = prob[0][0];
        choice[0][0][y] = 1;
    }

    int maxEforVehicle1 = std::min(E,M);
    float overloaded_p;
    for (int y = 1; y < maxEforVehicle1; y++) {
        // Final Probability is giving the energy at the current array element to that one vehicle.
        PROB[0][y][0] = prob[0][y];
        choice[0][y][0] = y+1;
        for(int i = 1; i < 3; i++) {
            overloaded_p = overloaded(prob[0][y - 1]); // Overload but with one less energy.
            if(prob[0][y] > overloaded_p) { // No overload is better.
                PROB[0][y][i] = prob[0][y];
                choice[0][y][i] = y+1;
            } else { // Overload with 1 less energy is better.
                PROB[0][y][i] = overloaded_p;
                choice[0][y][i] = -(y); // Reflects the one less energy (y+1 - 1).
            }
        }
    }
    for (int y = M; y < E; y++) { // Starts at M, or the index for having the energy of M + 1 (+1 is for overload).
        // Apply the maximum possible energy to the vehicle and overload it.
        PROB[0][y][0] = prob[0][M - 1];
        choice[0][y][0] = M;
        for(int i = 1; i < 3; i++) {
            PROB[0][y][i] = overloaded(prob[0][M-1]);
            choice[0][y][i] = -M;
        }
    }
        
    
    // Iteration
    float currentValue = -1;
    // Calculating the PROBability of success for vehicles 0..p given q + 1 energy.
    for (int p = 1; p < N; p++) {
        for (int q = p + 1; q < E; q++) {

            PROB[p][q][0] = -1;
            
            // Same as assign()
            for (int k = 0; k < min(M, q + 1 - p); k++) {
                
                currentValue = PROB[p - 1][q - k - 1][0] * prob[p][k];
                if(currentValue > PROB[p][q][0]) {
                    PROB[p][q][0] = currentValue;
                    choice[p][q][0] = k + 1;
                }
            }
        }
    }
    for (int p = 1; p < size_diagonal; p++) { 
        // If number of vehicles = energy, the optimal probability is the probability when there are no overloads.
        for(int overloads = 1; overloads < 3; overloads++) {
            PROB[p][p][overloads] = PROB[p][p][0];
            choice[p][p][overloads] = choice[p][p][0];
        }
    }
    for (int p = 1; p < N; p++) {
        for (int q = p + 1; q < E; q++) {

            // We might just use the previous (accounts for not overloading at all).
            PROB[p][q][1] = PROB[p][q][0]; // When you have one overload.
            choice[p][q][1] = choice[p][q][0];
            for (int k = 0; k < min(M, q + 1 - p - 1); k++) { // Check all if we don't overload yet.
                currentValue = PROB[p - 1][q - k - 1][1] * prob[p][k];
                if(currentValue > PROB[p][q][1]) {
                    PROB[p][q][1] = currentValue;
                    choice[p][q][1] = k + 1;
                }
            }
            for (int k = 0; k < min(M, q + 1 - p - 1); k++) { // Check all if we overload the current one.
                currentValue = PROB[p - 1][q - k - 2][0] * overloaded(prob[p][k]); // Reduce the remaining energy by 1 more for the overload cost.
                if(currentValue > PROB[p][q][1]) {
                    PROB[p][q][1] = currentValue;
                    choice[p][q][1] = -(k + 1); // While we are reducing the energy by 1, we are still giving k+1 energy to the current vehicle.
                }
            }
        }
    }
    for (int p = 1; p < size_diagonal; p++) { 
        // If number of vehicles = energy + 1, the optimal probability is the probability when there is only 1 overload.
        PROB[p][p+1][2] = PROB[p][p+1][1];
        choice[p][p+1][2] = choice[p][p+1][1];
    }
    for (int p = 1; p < N; p++) {
        for (int q = p + 2; q < E; q++) {

            // We can use the previous (accounts for not overloading at all or just one overload).
            PROB[p][q][2] = PROB[p][q][1]; // When you have two overloads.
            choice[p][q][2] = choice[p][q][1];

            for (int k = 0; k < min(M, q + 1 - p - 2); k++) { // Check all if we don't overload yet.
                currentValue = PROB[p - 1][q - k - 1][2] * prob[p][k];
                if(currentValue > PROB[p][q][2]) {
                    PROB[p][q][2] = currentValue;
                    choice[p][q][2] = k + 1;
                }
            }
            for (int k = 0; k < min(M, q + 1 - p - 2); k++) { // Check all if we overload the current one and still have one left.
                currentValue = PROB[p - 1][q - k - 2][1] * overloaded(prob[p][k]);
                if(currentValue > PROB[p][q][2]) {
                    PROB[p][q][2] = currentValue;
                    choice[p][q][2] = -(k + 1);
                }
            }
        }
    }

    p = PROB[N-1][E-1][2];
    
    // Get all choices of given energy to vehicles.
    int total_energy = E;
    int current_vehicle = N-1;
    int overloads_left = 2;
    while (total_energy > 0) {
        res0.push_back(choice[current_vehicle][total_energy - 1][overloads_left]);
        if (choice[current_vehicle][total_energy - 1][overloads_left] < 0) {
            total_energy += choice[current_vehicle][total_energy - 1][overloads_left]; // Add because it is negative.
            total_energy -= 1; // Subtract 1 as the cost of the overloading.
            overloads_left--;
        } else {
            total_energy -= choice[current_vehicle][total_energy - 1][overloads_left];
        }
        current_vehicle--;
    }
    std::reverse(res0.begin(), res0.end());

    return make_pair(res0, p);

}


 
 
