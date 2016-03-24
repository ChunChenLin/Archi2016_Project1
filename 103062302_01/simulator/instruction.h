#include "global.h"
#include "resolve.h"

inline void detectWrite2Zero(string format);
inline void detectNumberOverflow(string format, bool sub, bool isPos);
inline bool detectMemoryOverflow(int n);
inline bool detectDataMisaaligned(int n);

void R_format(string op);
void I_format(string op);
void J_format(string op);


