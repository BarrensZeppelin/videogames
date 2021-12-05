#include <string>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <iostream>

template<typename T, typename ID>
class ResourceStorage {
	std::unordered_map<ID, std::unique_ptr<T>> map;

	public:
		T& get(ID id) const {
			auto it = map.find(id);
			assert(it != map.end());
			return *it->second.get();
		}

		void load(ID id, std::string filename) {
			T* t = new T();
			if(!t->loadFromFile(filename)) {
				std::cerr << "Could not load " << filename << "!" << std::endl;
				assert(false);
			}

			map[id] = std::unique_ptr<T>(t);
		}

		void load(ID id, std::string a, std::string b) {
			T* t = new T();
			if(!t->loadFromFile(a, b)) {
				std::cerr << "Could not load " << a << ", " << b << "!" << std::endl;
				assert(false);
			}

			map[id] = std::unique_ptr<T>(t);
		}

};
