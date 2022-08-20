#include "files.h"

void save(std::vector<charge>& Charges) {
	if (Charges.size() > 0) {
		std::ofstream file;
		std::string f;
		std::cout << "Enter the file name to save : ";
		std::getline(std::cin, f);
		f += ".sim";
		file.open(f, std::ios::binary);
		std::vector<sf::Vector2f> data(Charges.size() * 3);
		for (int i = 0; i < Charges.size(); i++) {
			data[i * 3 + 0] = Charges[i].getpos();
			data[i * 3 + 1] = Charges[i].getvel();
			data[i * 3 + 2] = Charges[i].getacc();
		}
		unsigned int size = Charges.size(), dsize = data.size();
		std::streamsize nBytes = (Charges.size() * sizeof(charge)), cBytes = sizeof(size), pBytes = data.size() * sizeof(sf::Vector2f);
		const char* wBuff = reinterpret_cast<const char*>(&Charges[0]);
		const char* pBuff = reinterpret_cast<const char*>(&data[0]);
		const char* cBuff = reinterpret_cast<const char*>(&size);
		try {
			if (!file.write(cBuff, cBytes) ||
				!file.write(wBuff, nBytes) ||
				!file.write(pBuff, pBytes)) {
				throw 0;
			}
			else {
				throw 1;
			}
		}
		catch(int x) {
			if (x == 0) {
				std::cout << "unable to save" << std::endl;
			}
			else {
				std::cout << "file saved as '"<< f << "'"<< std::endl;
			}
		}
	}
	else {
		std::cout << "simulation is empty!" << std::endl;
	}
}

void load(std::vector<charge>& Charges) {
	std::ifstream file;
	std::string f;

	std::cout << "Enter the file name to load : ";
	std::getline(std::cin, f);
	Charges.clear();
	f += ".sim";
	file.open(f, std::ios::binary);

	if (file.is_open()) {
		unsigned int size = 0;
		char* cBuff = reinterpret_cast<char*>(&size);
		std::streamsize cBytes = sizeof(size);
		file.read(cBuff, cBytes);

		Charges = std::vector<charge>(size);
		(void)file.read(reinterpret_cast<char*>(&Charges[0]), sizeof(charge) * size);

		std::vector<sf::Vector2f> data(3 * size);
		(void)file.read(reinterpret_cast<char*>(&data[0]), 3 * size * sizeof(sf::Vector2f));
		for (int i = 0; i < size; i++) {
			Charges[i].setpos(data[i * 3 + 0]);
			Charges[i].setvel(data[i * 3 + 1]);
			Charges[i].setacc(data[i * 3 + 2]);
		}
		std::cout << "file '" << f << "' opened successfully" << std::endl;
	}
	else {
		std::cout << "file '" << f << "' unable to open" << std::endl;
	}
}

void saveImage(sf::RenderWindow& win) {
	std::string f;
	std::cout << "Enter the file name to save : ";
	std::getline(std::cin, f);
	f += ".bmp";
	sf::Texture tex;
	tex.create(win.getSize().x, win.getSize().y);
	tex.update(win);
	if (tex.copyToImage().saveToFile(f)) {
		std::cout << "Image saved as '" << f << "'" << std::endl;
	}
	else {
		std::cout << "Unable to save Image '" << f << "'" << std::endl;
	}
}