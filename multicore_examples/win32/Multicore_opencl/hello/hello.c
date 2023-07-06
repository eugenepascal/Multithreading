#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CL_TARGET_OPENCL_VERSION 200

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
  FILE* f = fopen(file_path, "rb");
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

  if(program_size)
  {
    *program_size = file_size;
  }
  fclose(f);
  return ret;
}

int main(int argc, char** argv)
{
  cl_platform_id platform;
  cl_uint nb_platforms = 0;
  cl_device_id device;
  cl_device_type device_type = CL_DEVICE_TYPE_CPU;
  cl_uint nb_devices = 0;
  cl_context context = NULL;
  cl_command_queue queue = NULL;
  cl_mem mem = NULL;
  cl_program program = NULL;
  cl_kernel kernel = NULL;
  size_t global_work_size[] = { 1 };
  size_t local_work_size[] = { 1 };
  char* program_data = NULL;
  size_t program_size = 0; 
  char str[1024];
  cl_int ret = -1;

  /* use first platform available */
  if(clGetPlatformIDs(1, &platform, &nb_platforms) != CL_SUCCESS ||
      nb_platforms == 0)
  {
    fprintf(stderr, "No OpenCL platform found\n");
    exit(EXIT_FAILURE);
  }

  /* use first device */
  if(clGetDeviceIDs(platform, device_type, 1, &device, &nb_devices)
      != CL_SUCCESS || nb_devices == 0)
  {
    fprintf(stderr, "No OpenCL device for platform found\n");
    exit(EXIT_FAILURE);
  }

  /* get OpenCL compute program source */
  program_data = opencl_get_program("hello.cl", &program_size);
  if(program_data == NULL)
  {
    fprintf(stderr, "Failed to get OpenCL program source code\n");
    exit(EXIT_FAILURE);
  }

  /* create context associated with device */
  context = clCreateContext(NULL, 1, &device, NULL, NULL, &ret);

#if CL_TARGET_OPENCL_VERSION < 200
  queue = clCreateCommandQueue(context, device, 0, &ret);
#else
  queue = clCreateCommandQueueWithProperties(context, device, NULL, &ret);
#endif

  mem = clCreateBuffer(context, CL_MEM_READ_WRITE, 1024 * sizeof(char), NULL,
      &ret);

  program = clCreateProgramWithSource(context, 1, (const char**)&program_data,
      &program_size,
      &ret);
  free(program_data);

  ret = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

  kernel = clCreateKernel(program, "hello", &ret);

  ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&mem);

  /* clEnqueueTask is deprecated and according to documentation:
   * clEnqueueTask is equivalent to calling clEnqueueNDRangeKernel with
   * work_dim = 1, global_work_offset = NULL, global_work_size[0] set to 1,
   * and local_work_size[0] set to 1.
   */
  /* ret = clEnqueueTask(queue, kernel, 0, NULL, NULL); */
  global_work_size[0] = 1;
  local_work_size[0] = 1;
  ret = clEnqueueNDRangeKernel(queue, kernel,
      1 , /* dim */
      NULL, /* global_work_offset */
      global_work_size,
      local_work_size,
      0, /* num_events_in_wait_list */
      NULL, /* event_wait_list */
      NULL/* event */);

  ret = clEnqueueReadBuffer(queue, mem, CL_TRUE, 0, 1024 * sizeof(char),
      str, 0, NULL, NULL);

  fprintf(stdout, "%s\n", str);

  clFlush(queue);
  clFinish(queue);
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseMemObject(mem);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);

  return EXIT_SUCCESS;
}

