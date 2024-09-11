#pragma once

#include "core/math/math.hpp"

#include <string>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace scsr
{

struct Face
{
    std::vector<Vec3i> data;

	Vec3i& operator[](usize index)
	{
		return data[index];
	}
};

struct Mesh
{
    Mesh() = default;
    Mesh(const std::string& path)
    {
        Load(path);
    }

    void Load(const std::string& path)
    {
		std::ifstream in;
		in.open(path, std::ifstream::in);
		if (in.fail()) return;
		std::string line;
		while (!in.eof()) {
			std::getline(in, line);
			std::istringstream iss(line.c_str());
			char trash;
			if (line.compare(0, 2, "v ") == 0) {
				iss >> trash;
				Vec3 v;
				for (int i = 0; i < 3; i++) iss >> v.data[i];
				vertices.push_back(v);
			}
			else if (line.compare(0, 2, "f ") == 0) {
				Face f;
				Vec3i tmp;
				iss >> trash;
				while (iss >> tmp.data[0] >> trash >> tmp.data[1] >> trash >> tmp.data[2]) {
					for (int i = 0; i < 3; i++) tmp.data[i]--;
					f.data.push_back(tmp);
				}
				faces.push_back(f);
			}
		}
    }

    std::vector<Vec3> vertices;
    std::vector<Face> faces;
};

}