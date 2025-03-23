#ifndef ATLAS_H
#define ATLAS_H

#include<vector>
#include<graphics.h>

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCSTR path_source,int num)
	{
		img_list.clear();
		img_list.resize(num);

		TCHAR path_template[256];
		for(int i=0;i<num;++i)
		{
			_stprintf_s(path_template, path_source, i + 1);
			loadimage(&img_list[i], path_template);
		}
	}
	void clear()
	{
		img_list.clear();
	}
	int get_size()
	{
		return (int)img_list.size();
	}
	IMAGE* get_image(int index)
	{
		if (index < 0 || index >= img_list.size())
			return nullptr;

		return &img_list[index];
	}
	void add_image(IMAGE& image)
	{
		img_list.push_back(image);
	}
private:
	std::vector<IMAGE> img_list;
};

#endif
