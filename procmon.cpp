#include "include_libraries.h"
#include "macro.h"

enum class Result
{
	successful,
	failure
};

struct my_tm
{
	time_t num_days = 0;
	time_t num_hours = 0;
	time_t num_minutes = 0;
	time_t num_seconds = 0;
	time_t all_seconds;
};

struct ContainerProcess
{
	DWORD pid = 0;
	wstr_t name = L"";
	my_tm worktime = { };
	long double memory = 0;
	double totalusingcpu = 0;
	double intervalusingcpu = 0;
};

class HandleRAII
{
	HANDLE _hProcess = nullptr;
public:
	explicit HandleRAII(HANDLE hProcess) noexcept : _hProcess(hProcess) { }

	explicit HandleRAII(HandleRAII&& other) noexcept : _hProcess(other._hProcess) 
	{
		other._hProcess = nullptr;
	}

	HandleRAII() noexcept = default;

	~HandleRAII()
	{
		if(_hProcess)
			CloseHandle(_hProcess);
	}

	HANDLE get() noexcept
	{
		return _hProcess;
	}

	HandleRAII& operator=(HANDLE hProcess)
	{
		if (_hProcess != hProcess)
		{
			if (_hProcess)
				CloseHandle(_hProcess);
			_hProcess = hProcess;
		}
		return *this;
	}

private:

	HandleRAII(const HandleRAII& obj) = delete;
};


// Получение ID процесса
namespace IdProc 
{
	// Доступ к методам и полям класса - родителя

	// Получение ID
	Result get(HANDLE& hProcess, const vec_t<DWORD>& aProcesses, size_t numProcess, ContainerProcess& Process)
	{
		DWORD ProcessPID = 0;

		// Проверка PID процесса на существование
		if (aProcesses[numProcess] != 0)
		{
			ProcessPID = aProcesses[numProcess];

			// Хэндл процесса
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
				FALSE, ProcessPID);

			if (!hProcess) return Result::failure;

			Process.pid = ProcessPID;

			return Result::successful;
		}

		return Result::failure;
	}
};

namespace NameProc
{
	// Получени имени процесса класса
	Result get(const HANDLE& hProcess, DWORD cbNeeded, ContainerProcess& Process)
	{
		// Хэндл модуля процесса
		HMODULE hMod = { };

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			wchar_t ProcessName[256];

			// Получение имени
			GetModuleBaseNameW(hProcess, hMod, ProcessName,
				256);

			// Проверка. не пуста ли строка
			if (wcslen(ProcessName) == 0) return Result::failure;

			Process.name = ProcessName;

			return Result::successful;
		}

		return Result::failure;
	}
};

namespace TimeProc
{
	// FILETIME в time_t
	Result filetime_to_time_t(time_t& t_tTime, const FILETIME fTime)
	{
		ULARGE_INTEGER ull;
		ull.LowPart = fTime.dwLowDateTime;
		ull.HighPart = fTime.dwHighDateTime;
		t_tTime = static_cast<time_t>(ull.QuadPart / 10000000ULL - 11644473600ULL);

		return Result::successful;
	}

	// Получение FILETIME в формате time_t
	Result getTime_t(const HANDLE& hProcess, time_t& t_tTimeProcess, const int choose)
	{
		FILETIME fCreationTimeProcess = { };
		FILETIME fKernelTimeProcess = { };
		FILETIME fExitTimeProcess = { };
		FILETIME fUserTimeProcess = { };

		if (!GetProcessTimes(hProcess, &fCreationTimeProcess, &fExitTimeProcess, &fKernelTimeProcess, &fUserTimeProcess)) return Result::failure;

		// Преобразование в локальное время из UTC
		if (!FileTimeToLocalFileTime(&fCreationTimeProcess, &fCreationTimeProcess) ||
			!FileTimeToLocalFileTime(&fExitTimeProcess, &fExitTimeProcess) ||
			!FileTimeToLocalFileTime(&fKernelTimeProcess, &fKernelTimeProcess) ||
			!FileTimeToLocalFileTime(&fUserTimeProcess, &fUserTimeProcess))
			return Result::failure;

		// Преобразование в системное время
		switch (choose)
		{
		case macCreateTimeProcess: filetime_to_time_t(t_tTimeProcess, fCreationTimeProcess); break;
		case macExitTimeProcess: filetime_to_time_t(t_tTimeProcess, fExitTimeProcess); break;
		case macKernelTimeProcess: filetime_to_time_t(t_tTimeProcess, fKernelTimeProcess); break;
		case macUserTimeProcess: filetime_to_time_t(t_tTimeProcess, fUserTimeProcess); break;
		default:;
		}

		return Result::successful;
	}

	// Время работы процесса
	Result getTimeWorkProcess(time_t& t_tWorkTimeProcess, const time_t t_tCreateProcess)
	{

		time_t t_tCurrentTime = time(NULL);

		if (t_tCurrentTime == -1) return Result::failure;

		// Разность времени текущего и времени старта
		t_tWorkTimeProcess = t_tCurrentTime - t_tCreateProcess;

		if (t_tWorkTimeProcess <= 0) return  Result::failure;

		return Result::successful;
	}

	// Преобразование времени в правильные форма
	Result time_t_to_my_tm(time_t t_tWorkTimeProcess, struct my_tm& my_tmWorkTimeProcess)
	{
		// Всё количество секунд
		my_tmWorkTimeProcess.all_seconds = t_tWorkTimeProcess;

		auto chrono_seconds = std::chrono::seconds(t_tWorkTimeProcess);
		// Количество дней
		auto num_days = std::chrono::duration_cast<std::chrono::hours>(chrono_seconds) / 24;
		my_tmWorkTimeProcess.num_days = num_days.count();

		auto num_hours = std::chrono::duration_cast<std::chrono::hours>(chrono_seconds) % 24;
		my_tmWorkTimeProcess.num_hours = num_hours.count();

		auto num_minutes = std::chrono::duration_cast<std::chrono::minutes>(chrono_seconds) % 60;
		my_tmWorkTimeProcess.num_minutes = num_minutes.count();

		auto num_seconds = std::chrono::duration_cast<std::chrono::seconds>(chrono_seconds) % 60;
		my_tmWorkTimeProcess.num_seconds = num_seconds.count();

		return Result::successful;
	}

	// Получение времени работы процесса
	Result get(const HANDLE& hProcess, ContainerProcess& Process)
	{
		struct my_tm my_tmWorkTimeProcess = { };
		time_t t_tCreateTimeProcess = 0;
		time_t t_tWorkTimeProcess = { };

		// Время в формате time_t
		if(getTime_t(hProcess, t_tCreateTimeProcess, macCreateTimeProcess) == Result::failure)
			return Result::failure;

		// Время работы процесса
		if(getTimeWorkProcess(t_tWorkTimeProcess, t_tCreateTimeProcess) == Result::failure)
			return Result::failure;

		// time_t в my_tm
		time_t_to_my_tm(t_tWorkTimeProcess, my_tmWorkTimeProcess);

		Process.worktime = my_tmWorkTimeProcess;

		return Result::successful;
	}
}

namespace UsingCpuProc
{
	Result getUllTime(const HANDLE& hProcess, ULARGE_INTEGER& TimeUsingProc)
	{
		FILETIME CreationTimeProcess = { };
		FILETIME ExitTimeProcess = { };
		FILETIME KernelTimeProcess = { };
		FILETIME UserTimeProcess = { };
		ULARGE_INTEGER KernelUnatedTime = { };
		ULARGE_INTEGER UserUnatedTime = { };

		if (!GetProcessTimes(hProcess, &CreationTimeProcess, &ExitTimeProcess, &KernelTimeProcess, &UserTimeProcess)) return Result::failure;

		KernelUnatedTime = { KernelTimeProcess.dwLowDateTime, KernelTimeProcess.dwHighDateTime };
		UserUnatedTime = { UserTimeProcess.dwLowDateTime, UserTimeProcess.dwHighDateTime };

		TimeUsingProc.QuadPart = KernelUnatedTime.QuadPart + UserUnatedTime.QuadPart;

		return Result::successful;
	}

	// Загруженность процессора за всё время существования
	Result getTotalUsingCpu(unsigned long num_cores, vec_t<ContainerProcess>& ContainerProcesses, vec_t<HandleRAII>& Handles)
	{
		int ErrorCode = 1;

		double TotalWorkTimeProcessor = 0;

		// Общее время жизни для текущего процесса
		ULARGE_INTEGER ullTime = { };

		double UsingCpu = 0;

		for (size_t index = 0; index < ContainerProcesses.size(); ++index)
		{
			// Ощее время работы процессора
			TotalWorkTimeProcessor = static_cast<double>(ContainerProcesses[index].worktime.all_seconds);

			if (getUllTime(Handles[index].get(), ullTime) == Result::failure)
			{
				ContainerProcesses[index].totalusingcpu = -1;
				continue;
			}

			// Проверка на ноль
			if (TotalWorkTimeProcessor <= 0)
			{
				ContainerProcesses[index].totalusingcpu = -1;
				continue;
			}

			// Загруженность за всё время и перевол секунд в 10ns
			TotalWorkTimeProcessor *= 1.0E7;

			UsingCpu = ullTime.QuadPart / (TotalWorkTimeProcessor * num_cores);

			UsingCpu *= 100.0;

			if (UsingCpu <= 0)
			{
				ContainerProcesses[index].totalusingcpu = -1;
				continue;
			}

			ContainerProcesses[index].totalusingcpu = UsingCpu;
		}

		return Result::successful;
	}

	// Загруженность за конкретный интервал времени
	Result getIntervalUsingCpu(unsigned long num_cores, long long IntervalWorkTimeProcessor, vec_t<ContainerProcess>& ContainerProcesses, vec_t<HandleRAII>& Handles)
	{
		vec_t<int> IndexesNotCorrectTimes = { };

		vec_t<ULONGLONG> vec_StartTimeProc{ };
		vec_t<ULONGLONG> vec_EndTimeProc{ };

		// Перераспределение памяти
		vec_StartTimeProc.resize(ContainerProcesses.size());
		vec_EndTimeProc.resize(ContainerProcesses.size());

		for (int index = 0; index < ContainerProcesses.size(); ++index)
		{
			// Получение текущей загруженности процессора
			ULARGE_INTEGER ullTime = { };
			
			if (getUllTime(Handles[index].get(), ullTime) == Result::failure)
			{
				IndexesNotCorrectTimes.push_back(index);
				continue;
			}

			vec_StartTimeProc[index] = ullTime.QuadPart;

			// Очищение структуры
			ullTime = { };
		}

		// Пауза на определенное время
		std::this_thread::sleep_for(std::chrono::milliseconds(IntervalWorkTimeProcessor));

		for (int index = 0; index < ContainerProcesses.size(); ++index)
		{
			if (std::find(IndexesNotCorrectTimes.begin(), IndexesNotCorrectTimes.end(), index) != IndexesNotCorrectTimes.end())
			{
				ContainerProcesses[index].intervalusingcpu = -1;
				continue;
			}

			// Получение загрузки процессора через определнный интервал
			ULARGE_INTEGER ullTime = { };

			if (getUllTime(Handles[index].get(), ullTime) == Result::failure)
			{
				ContainerProcesses[index].intervalusingcpu = -1;
				continue;
			}

			vec_EndTimeProc[index] = ullTime.QuadPart;

			// Очищение структуры
			ullTime = { };
		}

		// Перевод ms в 10ns
		IntervalWorkTimeProcessor *= static_cast<long long>(1.0E4);

		for (size_t index = 0; index < ContainerProcesses.size(); ++index)
		{
			if (IntervalWorkTimeProcessor <= 0 || std::find(IndexesNotCorrectTimes.begin(), IndexesNotCorrectTimes.end(), index) != IndexesNotCorrectTimes.end())
			{
				ContainerProcesses[index].intervalusingcpu = -1;
				continue;
			}

			// Текущее использование CPU
			double UsingCpu = (static_cast<double>(vec_EndTimeProc[index]) - static_cast<double>(vec_StartTimeProc[index])) / 
				(static_cast<double>(IntervalWorkTimeProcessor) * static_cast<double>(num_cores));

			// Перевод в проценты проценты
			UsingCpu *= 100.0;

			if (UsingCpu <= 0)
			{
				ContainerProcesses[index].intervalusingcpu = -1;
				continue;
			}

			ContainerProcesses[index].intervalusingcpu = UsingCpu;
		}

		return Result::successful;
	}


	Result get(vec_t<ContainerProcess>& process, long long IntervalWorkTimeProcessor, vec_t<HandleRAII>& Handles)
	{
		int ErrorCode = 1;

		// Количество количества логических потоков
		unsigned long num_cores = std::thread::hardware_concurrency();

		// ОБщее использоание CPU
		if (getTotalUsingCpu(num_cores, process, Handles) == Result::failure)
			return Result::failure;

		// За определенный интервал времени
		if(getIntervalUsingCpu(num_cores, IntervalWorkTimeProcessor, process, Handles) == Result::failure)
			return Result::failure;

		return Result::successful;
	}
}

namespace MemoryProc
{
// Получение ОЗУ процессора
	Result get(const HANDLE hProcess, ContainerProcess& Process)
	{
		PROCESS_MEMORY_COUNTERS pmc = { };

		if(!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) return Result::failure;

		long double memory = static_cast<long double>(pmc.WorkingSetSize);

		if (memory <= 0) return Result::failure;

		Process.memory = memory;

		return Result::successful;
	}
}

namespace ManageProgramm
{
	Result getParametersProcessor(DWORD& cbNeeded, DWORD& cProcesses, vec_t<DWORD>& aProcesses)
	{
		aProcesses.resize(8192);

		// Получение PID
		if (!EnumProcesses(aProcesses.data(), static_cast<DWORD>(aProcesses.size()) * sizeof(DWORD), &cbNeeded))
			return Result::failure;

		cProcesses = cbNeeded / sizeof(DWORD);

		// Если количество процессов равно 0
		if (!cProcesses) return Result::failure;

		return Result::successful;
	}

	Result getIndexesProcesses(size_t& start_index_process, size_t& end_index_process, const unsigned int max_threads, size_t num_thread,
		DWORD MaxProcessesOnOneThread, const DWORD cProcesses)
	{
		start_index_process = num_thread * MaxProcessesOnOneThread;
		end_index_process = (num_thread + 1) * MaxProcessesOnOneThread;

		if (num_thread == max_threads - 1)
			end_index_process += cProcesses - MaxProcessesOnOneThread * max_threads;

		return Result::successful;
	}

	Result getInformationAboutProcesses(const DWORD cbNeeded, const DWORD cProcesses, vec_t<DWORD>& aProcesses, size_t num_thread, DWORD MaxProcessesOnOneThread, 
		vec_t<vec_t<ContainerProcess>>& AllProcesses, const unsigned int max_threads)
	{
		// Информация о процессах
		vec_t<HandleRAII> Handles;
		ContainerProcess Process = { };
		
		// HANDLE для доступа к процессору и его PID-ы
		HANDLE hProcess = nullptr;
		HandleRAII Handle;

		// Индексы процесса
		size_t start_index_process;
		size_t end_index_process;
		getIndexesProcesses(start_index_process, end_index_process, max_threads, num_thread, MaxProcessesOnOneThread, cProcesses);
		size_t num_elements = end_index_process - start_index_process;
		
		AllProcesses[num_thread].reserve(num_elements);
		Handles.reserve(num_elements);

		for (size_t index = start_index_process; index < end_index_process; ++index)
		{
			// Стартовое значение переменных в текущей итерации
			Process = { };

			// PID
			if (IdProc::get(hProcess, aProcesses, index, Process) == Result::failure)
				continue;
			Handle = hProcess;

			// Name
			if(NameProc::get(hProcess, cbNeeded, Process) == Result::failure)
				Process.name = L"NoName";

			// Time
			if(TimeProc::get(hProcess, Process) == Result::failure)
				Process.worktime = { NULL, NULL, NULL, NULL, NULL };

			// Memory
			if(MemoryProc::get(hProcess, Process) == Result::failure)
				Process.memory = NULL;

			Handles.push_back(std::move(Handle));
			AllProcesses[num_thread].push_back(std::move(Process));
		}
		
		// Получение процента загрузки процессора для процесса, за всё время и за укзанный интервал
		if (UsingCpuProc::get(AllProcesses[num_thread], 1000, Handles) == Result::failure)
			return Result::failure;

		return Result::successful;
	}

	void start_programm()
	{
		const unsigned int max_threads = std::thread::hardware_concurrency() / 2;

		DWORD cbNeeded;
		DWORD cProcesses;
		vec_t<DWORD> aProcesses;
		vec_t<vec_t<ContainerProcess>> AllProcesses(max_threads);

		getParametersProcessor(cbNeeded, cProcesses, aProcesses);

		DWORD MaxProcessesOnOneThread = cProcesses / max_threads;

		vec_t<std::thread> threads;
		threads.resize(max_threads);

		AllProcesses.reserve(cProcesses);

		for (size_t num_thread = 0; num_thread < max_threads; ++num_thread)
		{
			threads[num_thread] = std::thread([cbNeeded, cProcesses, &aProcesses, num_thread, MaxProcessesOnOneThread, &AllProcesses, max_threads]() 
			{ 
					getInformationAboutProcesses(cbNeeded, cProcesses, aProcesses, num_thread, MaxProcessesOnOneThread, AllProcesses, max_threads); 
			});
		}

		for (auto& th : threads)
		{
			th.join();
		}
	};
}

int main() 
{
	auto start = std::chrono::steady_clock::now();

	ManageProgramm::start_programm();

	auto diff = std::chrono::steady_clock::now() - start;

	std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;


	return 0;
}