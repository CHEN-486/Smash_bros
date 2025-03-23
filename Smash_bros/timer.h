#ifndef	TIMER_H
#define	TIMER_H

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void restart()
	{
		pass_time = 0;
		shotted = false;
	}
	void set_wait_time(int wait_time)
	{
		this->wait_time = wait_time;
	}
	void set_one_shot(bool one_shot)
	{
		this->one_shot = one_shot;
	}
	void set_callback(std::function<void()> function)
	{
		callback = function;
	}
	void pause()
	{
		this->paused = true;
	}
	void resume()
	{
		this->paused = false;
	}
	void on_update(int delta)
	{
		if (paused)
			return;
		pass_time += delta;
		if (pass_time >= wait_time)
		{
			if (!one_shot || (one_shot && !shotted) && callback)
				callback();
			shotted = true;
			pass_time = 0;
		}
	}
private:
	int pass_time;
	int wait_time;
	bool paused = false;
	bool shotted = false;
	bool one_shot = false;
	std::function<void()> callback;
};
#endif
