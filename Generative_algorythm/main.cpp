# include <iostream>
# include <math.h>
# include <typeinfo>
# include <fstream>
# include <string>
# include <sstream>
# include <random>

using namespace std;

void generate_population(float** population, int pop_lenght, int ch_length);

float fitness_func(float* ind, int chromo_length);

void individuals_fitness(float** population, int population_length, int chromo_length, float* fitness_of_inds);

void make_selection(float** population, int population_length, float** selection);

void single_point_cb(float* parent1, float* parent2, int chromo_length, float* child);

void make_generation(float** selection, int population_length, int chromo_length, float** population);

void run_evolution(int n, float** population, float** selection, int population_length, int chromo_length);

void sort_inds_from_fitness(float* fitness_arr, int* indxs, float population_lenght);

void fill_indx(int* arr, int length);

int is_equal(float val1, float val2);

void make_ranks(float* fit_results, int* indxs, float population_lenght, float* ranks);

void make_rank_prob(float* ranks, int population_length, float* rank_probs);

void sort(float* arr, float size);

void show_matrix(float** arr, int rows, int columns);

void show_list(float* arr, int size);

class selection_method
{
private:
    /* data */
public:
    selection_method(/* args */);
    ~selection_method();
};

selection_method::selection_method(/* args */)
{
}

selection_method::~selection_method()
{
}


int main() {
    srand(time(0));
    
    // Параметры алгоритма

    int chromo_length = 10;
    int population_length = 10;

    int selection_type = 1; //1 - пропорциональная селекция; 2 - турнирная селекция; 3 - ранговая селекция
    float p_mutation = 1/float(chromo_length);
    int iterations = 1;
    
    // Популяция
    float** population;
    population = new float* [population_length];
    for (int ind = 0; ind < population_length; ind++) {
        population[ind] = new float [chromo_length];
    }
    // Селекция
    float** selection;
    selection = new float* [2 * population_length];
    for (int ind = 0; ind < 2 * population_length; ind++) {
        selection[ind] = new float [chromo_length];
    }
    // Оценки
    float* metrics;
    metrics = new float [population_length];

    // Лучшая хромосома
    float* best_chromo;
    best_chromo = new float [chromo_length];
    
    // Главный алгоритм
    
    generate_population(population, population_length, chromo_length);
    show_matrix(population, population_length, chromo_length);
    
    // run_evolution(n, population, selection, population_length, chromo_length);

    return 0;
}

//
// ОПРЕДЕЛЕНИЕ ФУНКЦИЙ
//

float fitness_func(float* ind, int chromo_length){
    float sum = 0;
    for (int i = 0; i < chromo_length; i ++){
        sum = sum + ind[i];
    }
    return sum;
}

void individuals_fitness(float** population, int population_length, int chromo_length, float* fitness_of_inds){
    for (int ind = 0; ind < population_length; ind++){
        fitness_of_inds[ind] = fitness_func(population[ind], chromo_length);
    }
}

void prop_selection(float** population, float* fit_results, int* indxs, int population_length, float** selection){
    
}

void single_point_cb(float* parent1, float* parent2, int chromo_length, float* child){
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution dist(0.5);
    int div_point = chromo_length / 2;
    
    for (int block1 = 0; block1 < div_point; block1++){
        child[block1] = parent1[block1];
    }
    for (int block2 = div_point; block2 < chromo_length; block2++){
        child[block2] = parent2[block2];
    }
}

int is_equal(float val1, float val2){
    if (val1 == val2){return int(1);}
    else {return float(0);}
}

void fill_indx(int* arr, int length){
    for (int i = 0; i < length; i++){
        arr[i] = i;
    }
}

void make_ranks(float* fit_results, int* indxs, float population_lenght, float* ranks){
    float numerator;
    float denominator;
    for (int i = 0; i < population_lenght; i++){
        numerator = 0;
        denominator = 0;
        
        for (int j = 0; j < population_lenght; j++){
            numerator += (i+1)*is_equal(fit_results[indxs[i]], fit_results[indxs[j]]);
        }
        for (int j = 0; j < population_lenght; j++){
            denominator += is_equal(fit_results[indxs[i]], fit_results[indxs[j]]);
        }
    
        ranks[i] = numerator/denominator;
    }
}

void make_rank_prob(float* ranks, int population_length, float* rank_probs){
    float sum = 0;
    for (int ind = 0; ind < population_length; ind++){
        sum += ranks[ind];
    }
    for (int ind = 0; ind < population_length; ind++){
        rank_probs[ind] = ranks[ind]/sum;
    }
}

void make_generation(float** selection, int population_length, int chromo_length, float** population){
    for (int ind = 2; ind < 2*population_length; ind++) {
        single_point_cb(selection[ind-2], selection[ind-1], chromo_length, population[ind/2 - 1]);
    }
}

void run_evolution(int ev_type, int iters, float** population, float** selection, int population_length, int chromo_length, float* best_ch){
    float best_score = 0;
    for (int epoch = 0; epoch < iters; epoch++){
        int* indxs = new int[population_length];
        float* fit_results = new float[population_length];
//        float* ranks = new float[population_length];
//        float* rank_probs = new float[population_length];
        
        fill_indx(indxs, population_length);
        individuals_fitness(population, population_length, chromo_length, fit_results);
        sort_inds_from_fitness(fit_results, indxs, population_length);
    
        prop_selection(population, fit_results, indxs, population_length, selection);
        
        if (fit_results[population_length-1] > best_score){best_score = fit_results[population_length-1];}
        
//        make_ranks(fit_results, indxs, population_length, ranks);
//        make_rank_prob(ranks, population_length, rank_probs);
//        sort(rank_probs, population_length);
//        rank_selection(population, population_length, selection);
        
        show_matrix(selection, 2*population_length, chromo_length);

        
        
        //make_generation(selection, population_length, chromo_length, population);
        
        
        
        
//        for (int ind = 0; ind < population_length; ind++)
//        {
//            fitness = fitness_func(population[ind], chromo_length);
//            if (best_score < fit_results[ind]){
//                best_score = fitness;
//            }
//        }
//        cout << "Популяция: " << epoch + 1 << ", лучший результат: " << best_score << endl;
    }
}

void sort_inds_from_fitness(float* fitness_arr, int* indxs, float population_lenght) {
    for (int i = 0; i < population_lenght - 1; ++i) {
        for (int j = 0; j < population_lenght - i - 1; ++j) {
            if (fitness_arr[j] > fitness_arr[j + 1]) {
                // Меняем пригодность местами
                float fit_temp = fitness_arr[j];
                fitness_arr[j] = fitness_arr[j + 1];
                fitness_arr[j + 1] = fit_temp;
                
                // Меняем хромосомы местами
                int ind_temp = indxs[j];
                indxs[j] = indxs[j + 1];
                indxs[j + 1] = ind_temp;
            }
        }
    }
}

void sort(float* arr, float size){
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Меняем местами
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void fill_indx(float* arr, float length){
    for (int i = 0; i < length; i++){
        arr[i] = i;
    }
}

void show_matrix(float** arr, int rows, int columns){
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++) {
            cout << arr[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void show_list(float* arr, int size){
    for (int i = 0; i < size; i++)
    {
        cout << arr[i];
    }
    cout << endl;
}

void generate_population(float** population, int pop_lenght, int ch_length){
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution dist(0.5);

    // Генерация хромосом
    for (int ind = 0; ind < pop_lenght; ind++)
    {
        for (int block = 0; block < ch_length; ++block) {
            population[ind][block] = dist(gen);
        }
    }
}
