#ifndef BACKING_STORE_H_
#define BACKING_STORE_H_

class OpenFile;

class BackingStore {
 public:
  BackingStore(int pageSize, int numPages, int fileNumber);
  ~BackingStore();

  bool isPagedOut(int virtualPageNumber);
  bool pageIn(int virtualPageNumber, int physicalPageNumber);
  bool pageOut(int virtualPageNumber, int physicalPageNumber);

 private:
  int locatePageInFile(int pageNumber);

  bool fileIsOpen;
  char* filename;
  OpenFile* openFile;
  bool* pagedOut;
  int pageSize;
  int numPages;
};

#endif /*BACKING_STORE_H_*/
