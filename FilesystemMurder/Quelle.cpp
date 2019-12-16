/// <summary>
/// Written by DVSProductions
/// Please be responsible.
/// </summary>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <random>
#include <stack>
#include <thread>
#include "randfast.h"
using namespace std;
#undef max
bool question(string q) {
	const string yes[] = { "1","true","y","yes" };
	const string no[] = { "0","false","n","no" };
	string s;
	while (true) {
		cout << q;
		cin >> s;
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
		for (uint_fast8_t n = 0; n != 4; n++) {
			if (s == yes[n])return true;
			else if (s == no[n])return false;
		}
		cout << "Please input a boolean value" << endl;
	}
}
uint64_t readnum(string q, uint64_t min = 0, uint64_t max = UINT64_MAX) {
	while (true) {
		cout << q;
		uint64_t input;
		cin >> input;
		if (input > min&& input < max)
			return input;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Please stay between " << min << " and " << max << endl;
	}
}
uint8_t* dataArr;
randfast* rng;
uint_fast64_t foldercount = 0, filecount = 0, iteration = 0, filesize, power;
string lastprint;
bool createFiles, createFolders, ruinSubfolders;
void genfile(string path, uint_fast64_t size) {
	for (uint_fast64_t n = 0; n != size; n++)
		dataArr[n] = static_cast<uint8_t>(rng->apply());
	ofstream o(path + ".bin", ios::out | ios::binary);
	for (uint_fast64_t n = 0; n != size; n++)
		o << dataArr[n];
	o.close();
	filecount++;
}
void genfolder(string path) {
	filesystem::create_directory(path);
	foldercount++;
}
__std_fs_stats& GetM1(filesystem::directory_entry* target) {
	return*(reinterpret_cast<__std_fs_stats*>(target));
}
std::string fn = "0";

void printstatus() {
	std::string ns = "\rIteration " + fn + " (files: " + to_string(filecount) + " Dirs: " + to_string(foldercount) + ")";
	cout << ns;
	for (size_t n = ns.length(); n < lastprint.length(); n++)
		cout << " ";
	lastprint = ns;
}
void statusprinter() {
	while (true) {
		using namespace chrono_literals;
		printstatus();
		this_thread::sleep_for(70ms);
	}
}
void Ruin(string path) {
	for (uint64_t n = 0; n != power; n++) {
		auto s = path + ' ' + std::to_string(n);
		if (createFolders)
			genfolder(s);
		if (createFiles)
			genfile(s, filesize);
	}
}
int main() {
	string s;
	do {
		cout << "Target Path: ";
		cin >> s;
	} while (!filesystem::exists(s) || !filesystem::is_directory(s));
	if (s[s.length()] != '\\')s += '\\';
	createFiles = question("Create Files? ");
	createFolders = question("Create folders? ");
	filesize = createFiles ? !question("Are files empty? ") ? question("Break files with blocksize? ") ? 1 : readnum("Enter filesize: ") : 0 : 0;
	ruinSubfolders = question("Ruin Subfolders? ");
	power = readnum("Enter Power: ");
	rng = new randfast(random_device()(), 0, 255);
	if (filesize != 0) {
		dataArr = new uint8_t[filesize];
	}
	uint_fast64_t fileidx = 0;
	stack<filesystem::directory_entry> st;
	std::thread t(statusprinter);
	while (true) {
		fn = std::to_string(iteration++);
		filecount = 0;
		foldercount = 0;

		if (ruinSubfolders) {
			for (const auto& entry : filesystem::directory_iterator(s))
				if (entry.is_directory()/*&& entry.status().permissions*/)
					st.push(entry);
			while (!st.empty()) {
				filesystem::directory_entry current = st.top();
				st.pop();
				if (current._Available(__std_fs_stats_flags::_Attributes) && _Bitmask_includes(GetM1(&current)._Attributes, __std_fs_file_attr::_System))
					continue;
				for (const auto& entry : filesystem::directory_iterator(current))
					if (entry.is_directory())
						st.push(entry);
				Ruin(current.path().string() + "\\" + fn);
			}
		}
		Ruin(s + fn);
	}
}

