#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <CL/cl.h>

/**
 * \brief Returns OpenCL file content.
 * \param file path of the OpenCL file.
 * \param program_size size of the file content.
 * \return content of the file or NULL if failure.
 * \note Caller MUST free the return value after use.
 */
static char* opencl_get_program(const char* file_path,
    size_t* program_size)
{
  char* ret = NULL;
  size_t file_size = 0;
  FILE* f = fopen(file_path, "r");
  size_t n = 0;

  assert(f);

  /* get the size of the content */
  fseek(f, 0, SEEK_END);
  file_size = ftell(f);
  rewind(f);

  ret = malloc(sizeof(char) * file_size + 1);

  if(!ret)
  {
    fclose(f);
    return ret;
  }

  n = fread(ret, sizeof(char), file_size, f);
  ret[n] = 0x00;

  if(n != file_size)
  {
    free(ret);
    ret = NULL;
  }

  *program_size = file_size;
  fclose(f);
  return ret;
}

int main(int argc, char** argv)
{
  cl_platform_id* platforms = NULL;
  cl_uint nb_platforms = 0;

  /* get the number of platforms */
  if(clGetPlatformIDs(0, NULL, &nb_platforms) == CL_SUCCESS)
  {
    if(nb_platforms > 0)
    {
      /* allocate memory and retrieves platforms */
      platforms = malloc(sizeof(cl_platform_id) * nb_platforms);
      if(platforms)
      {
        clGetPlatformIDs(nb_platforms, platforms, NULL);
      }
      else
      {
        fprintf(stderr, "Failed to allocate memory for platforms\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  else
  {
    fprintf(stderr, "No OpenCL platform found\n");
    exit(EXIT_FAILURE);
  }

  if(platforms == NULL)
  {
    fprintf(stderr, "No platform available\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Found %u platform(s)\n\n", nb_platforms);

  for(cl_uint i = 0 ; i < nb_platforms ; i++)
  {
    char buf[1024];
    cl_device_id* devices = NULL;
    cl_uint nb_devices = 0;

    /* show information of the platform */
    fprintf(stdout, "Platform %u\n", i);

    /* platform profile info */
    buf[0] = 0x00;
    clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, sizeof(buf),
        buf, NULL);
    fprintf(stdout, "\tCL_PLATFORM_PROFILE: %s\n", buf);

    /* platform version info */
    buf[0] = 0x00;
    clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(buf),
        buf, NULL);
    fprintf(stdout, "\tCL_PLATFORM_VERSION: %s\n", buf);

    /* platform name */
    buf[0] = 0x00;
    clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(buf),
        buf, NULL);
    fprintf(stdout, "\tCL_PLATFORM_NAME: %s\n", buf);

    /* platform vendor */
    buf[0] = 0x00;
    clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(buf),
        buf, NULL);
    fprintf(stdout, "\tCL_PLATFORM_VENDOR: %s\n", buf);

    /* platform extensions */
    buf[0] = 0x00;
    clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, sizeof(buf),
        buf, NULL);
    fprintf(stdout, "\tCL_PLATFORM_EXTENSIONS: %s\n", buf);
    fprintf(stdout, "\n");

    /* enumerate all devices for this platform */
    if(clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &nb_devices)
        == CL_SUCCESS)
    {
      if(nb_devices > 0)
      {
        devices = malloc(sizeof(cl_device_id) * nb_devices);
        if(devices)
        {
          clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, nb_devices, devices,
              NULL);
        }
      }
    }

    if(nb_devices == 0)
    {
      fprintf(stdout, "\tFound no devices\n");
      continue;
    }

    /* show information of all devices for this platform */
    for(cl_uint j = 0 ; j < nb_devices ; j++)
    {
      cl_bool available = 0;
      cl_uint address_bits = 0;
      cl_ulong mem_size = 0;
      cl_device_type device_type;

      fprintf(stdout, "\tDevice %u\n", j);

      buf[0] = 0x00;
      clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(buf), buf, NULL);
      fprintf(stdout, "\t\tCL_DEVICE_NAME: %s\n", buf);

      buf[0] = 0x00;
      clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR, sizeof(buf), buf, NULL);
      fprintf(stdout, "\t\tCL_DEVICE_VENDOR: %s\n", buf);

      buf[0] = 0x00;
      clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, sizeof(buf), buf, NULL);
      fprintf(stdout, "\t\tCL_DEVICE_VERSION: %s\n", buf);

      clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, sizeof(cl_device_type),
          &device_type, NULL);
      fprintf(stdout, "\t\tCL_DEVICE_TYPE: %ld\n", device_type);

      clGetDeviceInfo(devices[j], CL_DEVICE_AVAILABLE, sizeof(cl_bool),
          &available, NULL);
      fprintf(stdout, "\t\tCL_DEVICE_AVAILABLE: %d\n", available);

      clGetDeviceInfo(devices[j], CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint),
          &address_bits, NULL);
      fprintf(stdout, "\t\tCL_DEVICE_ADDRESS_BITS: %d\n", address_bits);

      buf[0] = 0x00;
      clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, sizeof(buf), buf, NULL);
      fprintf(stdout, "\t\tCL_DEVICE_EXTENSIONS: %s\n", buf);

      clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong),
          &mem_size, NULL);
      fprintf(stdout, "\t\tCL_DEVICE_MEM_SIZE: %ld\n", mem_size);

      buf[0] = 0x00;
      clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, sizeof(buf), buf, NULL);
      fprintf(stdout, "\t\tCL_DRIVER_VERSION: %s\n", buf);
      fprintf(stdout, "\n");
    }

    free(devices);
  }

  /* create context for each platform, program and execute a kernel via
   * a queue
   */
  for(cl_uint i = 0 ; i < nb_platforms ; i++)
  {
    char* program_data;
    size_t program_size = 0;
    cl_int ret = 0;
    cl_context context;
    cl_uint refcount = 0;
    cl_context_properties context_props[] =
    {CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i], 0 };
    cl_device_id* devices = NULL;
    cl_uint nb_devices = 0;
    cl_uint nb_kernels = 0;
    cl_program program;
    cl_kernel kernel;
    cl_kernel* kernels = NULL;
    cl_command_queue queue;
    char buf[2048];
    size_t size = 0;
    int idx_fct = -1;

    context = clCreateContextFromType(context_props, CL_DEVICE_TYPE_ALL, NULL,
        NULL, &ret);

    if(ret != CL_SUCCESS)
    {
      fprintf(stderr, "Failed to create a context for platform %u\n\n", i);
      continue;
    }

    fprintf(stdout, "Context for platform %u\n", i);

    clGetContextInfo(context, CL_CONTEXT_DEVICES, 0,
        NULL, &size);
    if(size > 0)
    {
      devices = malloc(size);
      nb_devices = size / sizeof(cl_device_id);
      clGetContextInfo(context, CL_CONTEXT_DEVICES, size, devices, NULL);
      fprintf(stdout, "\t%u device(s)\n", nb_devices);
    }
    else
    {
      nb_devices = 0;
    }

    if(nb_devices == 0)
    {
      fprintf(stdout, "\tNo devices\n");
      clReleaseContext(context);
      continue;
    }

    /* show info of the context */
    clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint),
        &refcount, NULL);
    fprintf(stdout, "\tCL_CONTEXT_REFERENCE_COUNT: %u\n", refcount);

    /* get the OpenCL data */
    program_data = opencl_get_program("info.cl", &program_size);

    if(program_data == NULL)
    {
      fprintf(stderr, "\tBad read of OpenCL file\n");
      free(devices);
      clReleaseContext(context);
      continue;
    }

    program = clCreateProgramWithSource(context, 1, (const char**)&program_data,
        &program_size, &ret);

    free(program_data);

    if(ret != CL_SUCCESS)
    {
      fprintf(stderr, "\tFailed to create program\n");
      free(devices);
      clReleaseContext(context);
      continue;
    }

    clGetProgramInfo(program, CL_PROGRAM_SOURCE, sizeof(buf), buf, NULL);
    buf[sizeof(buf) - 1] = 0x00;
    fprintf(stdout, "%s\n", buf);

    if(clBuildProgram(program, nb_devices, devices, NULL, NULL, NULL) !=
        CL_SUCCESS)
    {
      char* log = NULL;

      clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
      log = malloc(sizeof(char) * size);
      fprintf(stdout, "Build of the program failed\n");

      if(log)
      {
        clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, size, log, NULL);
        log[size] = 0x00;
        fprintf(stdout, "%s\n", log);
        free(log);
      }
    }

    /* create the queue to communicate with the device */
#if CL_TARGET_OPENCL_VERSION < 200
    queue = clCreateCommandQueue(context, devices[0], 0, &ret);
#else
    queue = clCreateCommandQueueWithProperties(context, devices[0], NULL, &ret);
#endif

    if(ret != CL_SUCCESS)
    {
      fprintf(stdout, "Failed to create queue: %d\n", ret);
      free(devices);
      clReleaseContext(context);
      continue;
    }

    if(clCreateKernelsInProgram(program, 0, NULL, &nb_kernels) == CL_SUCCESS)
    {
      kernels = malloc(sizeof(cl_kernel) * nb_kernels);
      if(kernels)
      {
        clCreateKernelsInProgram(program, nb_kernels, kernels, NULL);
      }
      else
      {
        nb_kernels = 0;
      }
    }

    if(nb_kernels == 0)
    {
      fprintf(stderr, "\t\tNo kernels found in program\n");
      free(devices);
      clReleaseCommandQueue(queue);
      clReleaseProgram(program);
      clReleaseContext(context);
      continue;
    }

    fprintf(stdout, "\tKernel(s) available:\n");
    for(cl_uint j = 0 ; j < nb_kernels ; j++)
    {    
      cl_uint num_args = 0;
      clGetKernelInfo(kernels[j], CL_KERNEL_FUNCTION_NAME, sizeof(buf), buf, NULL);
      clGetKernelInfo(kernels[j], CL_KERNEL_NUM_ARGS, sizeof(cl_uint),
          &num_args, NULL);

      fprintf(stdout, "\t\tCL_KERNEL_FUNCTION_NAME: %s ", buf);
      fprintf(stdout, "\t\tCL_KERNEL_NUM_ARGS: %u\n", num_args);

      if(!strcmp(buf, "blank"))
      {
        idx_fct = j;
      }
    }

#if 1 
    if(idx_fct == -1)
    {
      ret = CL_INVALID_KERNEL_NAME;
    }
    else
    {
      kernel = kernels[idx_fct];
      ret = CL_SUCCESS;
    }
#else
    /* retrieve the function blank in the program */
    kernel = clCreateKernel(program, "blank", &ret);
#endif

    if(ret == CL_SUCCESS)
    {
      size_t global_work_size[1];
      size_t local_work_size[1];

      /* clEnqueueTask is deprecated and according to documentation:
       * clEnqueueTask is equivalent to calling clEnqueueNDRangeKernel with
       * work_dim = 1, global_work_offset = NULL, global_work_size[0] set to 1,
       * and local_work_size[0] set to 1.
       */
      /* if(clEnqueueTask(queue, kernel, 0, NULL, NULL) != CL_SUCCESS) */
      global_work_size[0] = 1;
      local_work_size[0] = 1;
      if(clEnqueueNDRangeKernel(queue, kernel,
          1 , /* dim */
          NULL, /* global_work_offset */
          global_work_size,
          local_work_size,
          0, /* num_events_in_wait_list */
          NULL, /* event_wait_list */
          NULL/* event */) != CL_SUCCESS)
      {
        fprintf(stderr, "\tFailed to enqueue task\n");
      }
      else
      {
        fprintf(stdout, "\tEnqueue tasks success\n");
      }

#if 0
      clReleaseKernel(kernel);
#endif
    }
    else
    {
      fprintf(stderr, "\tFailed to get kernel \"blank\": %d\n", ret);
    }

    /* release kernels allocated */
    for(cl_uint j = 0 ; j < nb_kernels ; j++)
    {
      clReleaseKernel(kernels[j]);
    }

    free(kernels);
    free(devices);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);
    fprintf(stdout, "\n");
  }

  free(platforms);

  fprintf(stdout, "End\n");
  return EXIT_SUCCESS;
}

