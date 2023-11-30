#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;

pair< vector<int>, float >  assign(int N, int E, int M, float **prob)
{
    vector <int> res0;
    float p = 1;

    if (E < N) {
        throw std::invalid_argument("The rescue fails because there is not enough energy to supply at least 1 energy to each vehicle!");
    }

    E = std::min(E, M*N); // Any energy beyond the value of M*N will not lead to any greater chance of success.

    // Base Cases have been checked.
    if (N == 1) { // Do we have to account for E or M being negative?
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
            // cout << "Calculating PROB at p=" << p << " and q=" << q << "\n";
            // cout << "\tTotal Energy: " << q + 1 << "\n";

            PROB[p][q] = -1;
            /**
             * q + 1 - p is the loop end because p needs to be <= q as each vehicle needs at least 1 energy.
             * M is the loop end because prob[p][k > M - 1] is equal to prob[p][M-1] and the PROBability of the other vehicles will be lower.
             *  Assuming that decreasing energy always decreases the likelihood of success.
            */
            for (int k = 0; k < min(M, q + 1 - p); k++) {
                // cout << "\t" << k+1 << " and " << q - k << " energy: " << "";
                // cout << PROB[p - 1][q - k - 1] << " * " << prob[p][k] << "\n";
                
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
    
    // cout << "Printing Full Array.\n";
    // for (int p = 0; p < N; p++) {
    //     for (int q = 0; q < E; q++) {
    //         cout << PROB[p][q] << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";
    // cout << "Printing Choice Array.\n";
    // for (int p = 0; p < N; p++) {
    //     for (int q = 0; q < E; q++) {
    //         cout << choice[p][q] << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";

    return make_pair(res0, p);

}


pair< vector<int>, float >  assign2(int N, int E, int M, float **prob)
{
    vector <int> res0;
    float p = prob[N-1][M-1];

    for (int i =0; i < N; i++)
        res0.push_back(0);	
    return make_pair(res0, p);

}


 
 
