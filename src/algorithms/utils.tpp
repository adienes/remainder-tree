#include <cassert>
#include <sstream>

#include "../elements/element.hpp"

using std::vector;


std::string replace_all_substr(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}


std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

std::map<long, long> parse_matrix_entry(std::string entry) {
	//An entry looks like "a_n*x^n + ... a_0"
    std::map<long ,long> polynomial;
    std::string mangled = replace_all_substr(entry, " ", "");   // delete spaces
    mangled = replace_all_substr(entry, "-", "+-");             // separate monomials with +
    mangled = replace_all_substr(mangled, "++", "+");           // merge duplicated + signs 
    mangled = replace_all_substr(mangled, "x", "x^1");          // add power of 1 to x term
    mangled = replace_all_substr(mangled, "^1^", "^");          // remove power of 1 to non x terms
    mangled = replace_all_substr(mangled, "x", "*x");           // add multiplication sign to all coefficients
    mangled = replace_all_substr(mangled, "**", "*");           // remove duplicated multiplication signs
    if (mangled.front() == '*') {
        mangled.insert(mangled.begin(), '+');           // add plus sign to beginning if monic to be covered by next replacement
    }
    mangled = replace_all_substr(mangled, "+*", "+1*");        // add 1 to all monic monomials
    mangled = replace_all_substr(mangled, "-*", "-1*");        // add -1 to all negative monic monomials
    mangled = replace_all_substr(mangled, "*x^", ",");          // replace *x^ with a single character separator 

    std::vector<std::string> tokenized = split(mangled, '+');
    tokenized.erase(tokenized.begin()); //Because there is always a leading +

    for (auto&& c : tokenized) {
        std::cout << c << ", ";
    }
    std::cout << std::endl;

    for (auto&& c : tokenized) {

        std::vector<std::string> cepair = split(c, ',');

        for (auto&& b : cepair) {
            std::cout << b << " ";
        }
        std::cout << std::endl;
        
        polynomial.insert(std::pair<long, long>(stoi(cepair[1]), stoi(cepair[0])));
        //  std::string::size_type exp = 0;

        //  std::cout << "c = " << c << std::endl;
        //  std::cout << "and the integer version!" << std::endl;
        //  std::cout << std::stoll(c, &exp) << " coeff " << std::endl;;

        //  std::cout << "exponent = ? " << c.substr(exp) << std::endl;
    }
    return polynomial;
}



PolyMatrix parse_matrix_formula(std::string formula) {
	PolyMatrix output;

	std::stringstream ss(formula);
	for (int i; ss >> i;) {
		std::cout << i << " ";
		if(ss.peek() == ',') {
			ss.ignore();
		}
	}
	return output;
}


bool is_power2(long x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

long flatten(long i , long j) {
	return (1<<i)+j;
}

template <typename T>
vector<T> get_subtree (const vector<T>& tree, long k) {
	long N = tree.size();

	vector<T> subtree(N);

	long i = 0;
	while ((1<<i)*(k+1) <= N) { //up through the leaves 
		for (long j = 0; j < (1<<i); ++j) {

			long node = flatten(i,j);
			subtree[node] = tree[node];
		}
		++i;
	}

	subtree.shrink_to_fit();
	return subtree;
}

template <typename T>
vector<T> get_shade(const vector<T>& vals, long k) {
	long N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		vector<T> slice(1);
		slice[0] = vals[k-N];
		return slice;
	}

	long left = k; //this will be the leftmost leaf under the subtree rooted at P
	long right = k; //similarly the rightmost leaf under subtree rooted at P
	while (left < N) { //stops when left_span is a leaf
		left = 2*left;
		right = 2*right + 1;

	}
	left -= N;
	right -= N;

	typename vector<T>::const_iterator first = vals.begin() + left;
	typename vector<T>::const_iterator last = vals.begin() + right + 1;
	vector<T> slice(first, last);

	return slice;
}


template<typename T>
vector<T> elt_to_base(const vector<Elt<T>> &e) {
	long N = e.size();

	vector<T> b(N);

	for(long i = 0; i < N; ++i) {
		b[i] = e[i].t;
	}

	return b;
}

template<typename T>
vector<Elt<T>> base_to_elt(const vector<T>& b) {
	long N = b.size();

	vector<Elt<T>> e(N);

	for(long i = 0; i < N; ++i) {
		e[i] = b[i];
	}

	return e;
}
