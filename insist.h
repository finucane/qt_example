/*
 * insist.h
 * simple assert macro that throws a descriptive exception on assertion failure
*/

#ifndef insist_h
#define insist_h

class InsistException
{
  public:
  InsistException (const char*fmt,...);
  const char*getMessage ()
  {
    return message;
  }
  private:
  enum
  {
    MAX_MESSAGE=255
  };
  char message [MAX_MESSAGE];
};

#if 1
#define insist(e) if(!(e)){throw InsistException ("assertion failed %s:%d (%s)", __FILE__, __LINE__, #e);}
#else
#define insist(e)((void)(e))
#endif

#endif // insist_h
