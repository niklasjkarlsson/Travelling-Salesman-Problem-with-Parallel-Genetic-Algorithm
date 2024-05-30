#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <sstream>
using namespace std;



// Structure to hold information of a city
struct City
{
  int idx;
  float x;
  float y;
};


// Initialize some variables
int population_size = 100;
int generations = 1000;
int mut_freq = 5;
int mig_freq =20;

string fname;

int chromosome_length;
vector<vector<float>> dist_matrix;
vector<City> cities;
set<int> geneset;
mt19937 gen;
uniform_int_distribution<int> chrom_dist;
uniform_int_distribution<int> pop_dist;


// Calculates the distance between two cities
float d(City i, City j)
{
  return sqrt( pow(j.x - i.x, 2) + pow(j.y - i.y, 2) );
}


static int getRandomSetElement(const set<int>& s) {
    // Check if the set is empty
    if (s.empty()) {
        cerr << "Set is empty!" << endl;
        return -1;
    }


    // Generate a random index within the size of the set
    uniform_int_distribution<> dist(0,s.size()-1);
    int randomIndex = dist(gen);

    // Create an iterator pointing to the random element
    auto it = next(s.begin(), randomIndex);

    // Return the element at the random index
    return *it;
}


// Class to hold and manipulate chromosome data
class Chromosome
{
public:
  vector<int> genes;
  float fitness;

  // Initialization with cities in order
  Chromosome(){
    genes.resize(chromosome_length);

    for (int i=0; i<chromosome_length; i++)
      genes[i] = i+1;
  }


  // Shuffle the cities to random order
  void permute()
  {
    shuffle(genes.begin(), genes.end(), gen);
  }


  // Mutate the chromosome
  void mutate()
  {
    unsigned int r_num1 = chrom_dist(gen);
    unsigned int r_num2 = chrom_dist(gen);
    int tmp = genes[r_num1];
    genes[r_num1] = genes[r_num2];
    genes[r_num2] = tmp;
  }
    
  
  // Calculate fitness of the chromosome
  void calculate_fitness()
  {
    fitness = 0;

    for (int i = 0; i < chromosome_length-1; i++)
      fitness += dist_matrix[genes[i] -1][genes[i+1] -1];

    fitness += dist_matrix[genes[chromosome_length-1]-1][0];
  }


  // Check if city exists in interval [0,k] of the chromosome
  bool check_if_city_exists(int city_id, int k)
  {
    set<int> city_set(genes.begin(), genes.begin() + k);
    bool exists = city_set.find(city_id) != city_set.end();
    return exists;
  }
    

  // Generate a child of this and given chromosome
  Chromosome generate_child(Chromosome *chrom)
  {
    Chromosome child;
    set<int> pool = geneset;
    child.genes[0] = genes[0];
    pool.erase(genes[0]);
    for (int i=1; i<chromosome_length; i++) {
      
      if (child.check_if_city_exists(genes[i],i)) {
        if (child.check_if_city_exists(chrom->genes[i],i)) {
	    int r_member = getRandomSetElement(pool);
	    child.genes[i] = r_member;
	    pool.erase(r_member);
	    continue;
	} else {
	    child.genes[i] = chrom->genes[i];
	    pool.erase(child.genes[i]);
	    continue;
	}
	
      } else if (child.check_if_city_exists(chrom->genes[i],i)) {
        child.genes[i] = genes[i];
	pool.erase(child.genes[i]);
	continue;
	
      } else {
	float dist1 = dist_matrix[child.genes[i-1]-1][genes[i]-1];
	float dist2 = dist_matrix[child.genes[i-1]-1][chrom->genes[i]-1];
	if (dist1 <= dist2) {
	  child.genes[i] = genes[i];
	  pool.erase(child.genes[i]);
	} else {
	  child.genes[i] = chrom->genes[i];
	  pool.erase(child.genes[i]);
	}
	
      }
      
    }
    return child;
  }


  // Print fitness of chromosome
  void print_fitness()
  {
    printf("Fitness: %f\n", fitness);
  }
    

  // This defines < operation for Chromosome for sorting
  friend bool operator<(const Chromosome& a, const Chromosome& b){ return a.fitness < b.fitness;}
};


// Output for a Chromosome
ostream& operator<<(ostream& out, const Chromosome& f)
{
  vector<int>::const_iterator it;

  for (it = f.genes.begin(); it != f.genes.end(); it++)
    out << *it << " ";
  out << "Fitness: " << f.fitness;
  return out << endl;
}

// Read city coordinates from input file
void read_data()
{
  int num_cities;
  float x, y;
  
  ifstream infile;
  infile.open(fname);

  string line_str;
  getline(infile, line_str);
  if (sscanf(line_str.c_str(), "DIMENSION : %d", &num_cities) == 0) {
        cerr << "Illegal TSP locations file format. Expecting the DIMENSION at line 1." << endl;
        exit(0);
  }
  chromosome_length = num_cities;
    
  cities.resize(num_cities);
  int index = 0;
    while (infile >> x >> y) {
        if (index < num_cities) {
            cities[index].idx = index + 1;
            cities[index].x = x;
            cities[index].y = y;
	    geneset.insert(index + 1);
	    index++;
        }
    }
    infile.close();
}


// Write chromosome to file
void save_chromosome(Chromosome &chrom)
{
  ostringstream oss;
  oss << "../run/best_chromosome" << chromosome_length << ".dat";
  string outfname = oss.str();
  ofstream outfile;
  outfile.open(outfname);

  if (outfile.is_open()) {
    outfile << "DIMENSION : " << chromosome_length << endl;
    outfile << "Fitness : "<< chrom.fitness << endl;
    for (auto it = chrom.genes.begin(); it != chrom.genes.end(); ++it) {
      outfile << cities[*it-1].idx << " " << cities[*it-1].x << " " <<  cities[*it-1].y << endl;
    }
    outfile << "EOF" << endl;
  } else {
    cout << "Not able to open output file\n";
    exit(0);
  }
  outfile.close();
}



// Prints distance matrix
void print_dist_matrix()
{
  printf("\n\t");
  for (int i = 0; i < chromosome_length; ++i){
    printf("\n|%d|\t", i);
    for (int j = 0; j < chromosome_length; j++)
      printf("(%d,%d) = %.4f\t",i,j, dist_matrix[i][j]);
  }
  printf("\n");
}

// Initializes a matrix that stores distances between every city
void initialize_dist_matrix()
{
  read_data();

  dist_matrix.resize(chromosome_length, vector<float>(chromosome_length));

  for (int i = 0; i < chromosome_length - 1; i++) {
    for (int j = i + 1; j < chromosome_length; j++) {
      float dist = d(cities[i], cities[j]);
      dist_matrix[i][j] = dist_matrix[j][i] = dist;
    }
  }
}




// Send every chromosome to the function above
void calculate_population_fitness(vector<Chromosome> &population)
{
  for (int i=0; i < population_size; i++)
    population[i].calculate_fitness();
}
  

// Swap two chromosomes
void swap_chromosomes(vector<Chromosome> *population, int src, int dest)
{
  Chromosome chrom = (*population)[src];
  (*population)[src] = (*population)[dest];
  (*population)[dest] = chrom;
}


// Sort population by fitness
void sort_population(vector<Chromosome>& population)
{
  sort(population.begin(), population.end());
}


// Shuffle order of population
void shuffle_population(vector<Chromosome> population)
{
  shuffle(population.begin(), population.end(), gen);
}


// Get random chromosome index
int get_rand_chrom_idx()
{
  return pop_dist(gen);
}


// Perform crossover for pair of chromosomes
void pair_crossover(Chromosome& a, Chromosome& b)
{
  Chromosome tmp = a.generate_child(&b);
  b = b.generate_child(&a);
  a = tmp;
}

void population_crossover(vector<Chromosome>& population)
{
  for (int i=0; i+1<population_size; i+=2){
    pair_crossover(population[i],population[i+1]);
  }
}
  


int main(int argc, char* argv[])
{
  if (argc != 6) {
    cerr << "Wrong number of arguments, expected 4:" << endl;
    cerr << argv[0] << " <filename> <population_size> <generations> <mutation_interval> <migration_interval>"<< endl;
    exit(0);
  }
  fname = argv[1];
  population_size = stoi(argv[2]);
  generations = stoi(argv[3]);
  mut_freq = stoi(argv[4]);
  mig_freq = stoi(argv[5]);
  
  
  
  const int seed=1;
  gen.seed(seed);
  


  initialize_dist_matrix();

  pop_dist = uniform_int_distribution<int>(0, population_size - 1);
  chrom_dist = uniform_int_distribution<int>(0, chromosome_length - 1);

  
  vector<Chromosome> population(population_size);

  
  
  for (int i=0; i<population_size; i++) {
    Chromosome chrom;
    chrom.permute();
    chrom.calculate_fitness();
    population[i] = chrom;  
  }

  sort_population(population);

  
  // Remember best chromosome
  Chromosome best_chrom = population[0];

  for (int i=0; i<generations; i++) {
    shuffle_population(population);
    population_crossover(population);
    if (i%mut_freq==0) {
      int r_c = pop_dist(gen);
      population[r_c].mutate();
    }
    if (i%mig_freq==0) {
      calculate_population_fitness(population);
      sort_population(population);
      if (population[0] < best_chrom) {
	best_chrom = population[0];
      }
      if (population[0].fitness != best_chrom.fitness) {
	population[population_size-1] = best_chrom;
      }
      cout << i << " " << population[0].fitness << endl;
    }
  }
  calculate_population_fitness(population);
  sort_population(population);
  save_chromosome(best_chrom);

  for (int i=0; i<population_size; i++) {
    cout << population[i];
  }  
  
  return 0;
}
