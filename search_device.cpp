#include "library.h"

cl_device_id search_device(int deviceKey)
{

	cl_uint platformCount = 0;
	clGetPlatformIDs(0, nullptr, &platformCount);

	vector<cl_platform_id> platforms(platformCount);
	clGetPlatformIDs(platformCount, platforms.data(), nullptr);

	vector<cl_uint> deviceCount(platformCount);
	for (int i = 0; i < platformCount; i++)
	{
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount[i]);
	}

	int deviceAmount = 0;
	for (int i = 0; i < platformCount; i++)
	{
		deviceAmount += deviceCount[i];
	}

	vector<cl_device_id> devicesAll(deviceAmount);
	for (int i = 0; i < deviceAmount; i++)
	{
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount[i], &devicesAll[i], NULL);
	}

	cl_device_id device = 0;
	cl_device_type deviceType;
	vector<cl_device_id> deviceSort(deviceAmount);
	int temp1 = 2;
	int temp2 = 0;
	for (int i = 0; i < deviceAmount; i++)
	{
		clGetDeviceInfo(devicesAll[i], CL_DEVICE_TYPE, sizeof(cl_device_type), &deviceType, NULL);
		if (deviceType == CL_DEVICE_TYPE_CPU)
		{
			deviceSort[deviceAmount - 1] = devicesAll[i];
		}
		else
		{
			cl_bool isMemoryUnified;
			clGetDeviceInfo(devicesAll[i], CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(cl_bool), &isMemoryUnified, NULL);
			if (isMemoryUnified)
			{
				deviceSort[deviceAmount - temp1] = devicesAll[i];
				temp1++;
			}
			else
			{
				deviceSort[temp2] = devicesAll[i];
				temp2++;
			}
		}
	}

	if (deviceKey >= deviceAmount)
	{
		deviceKey = 0;
	}

	device = deviceSort[deviceKey];
	return device;
}