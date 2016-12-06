#include <algorithm>
#include <cstdio>
#include <limits>
#include <string>
  
#include "stdint.h"
 
#include "Pipes.hh"
#include "TemplateFactory.hh"
#include "StringUtils.hh"

#include "utils.hh"
 
using namespace std;

class PageRankMapper : public HadoopPipes::Mapper {
public:
  PageRankMapper( HadoopPipes::TaskContext& context ) {}

  void map( HadoopPipes::MapContext& context ) {
    string line = context.getInputValue();

    vector< string > params =
      HadoopUtils::splitString( line, "\t " );
    string user = params[0];

    double rank = HadoopUtils::toFloat(params[1]);

    if ( params[2] != string("-") ) {
      vector< string > followings = 
        HadoopUtils::splitString( params[2], "," );

      double P = rank / followings.size();
      for (unsigned i = 0; i < followings.size(); ++i) 
      if ( followings[i].length() > 0 ) {
        context.emit( followings[i], doubleToString(P) );
      }
    }

    context.emit( user, params[1] + " " + params[2] );
  }
};
 
class PageRankReducer : public HadoopPipes::Reducer {
private:
  static double D;

public:
  PageRankReducer(HadoopPipes::TaskContext& context) {}

  void reduce( HadoopPipes::ReduceContext& context ) {
    double rank = 0.0;
    string followings;

    while ( context.nextValue() ) {
      vector< string > line = 
        HadoopUtils::splitString( context.getInputValue(), " " );

      if ( line.size() == 1 ) {
        rank += HadoopUtils::toFloat(line[0]);
      } else {
        followings = line[1];
      }
    }

    rank = D * rank + 1 - D;
    context.emit(context.getInputKey(), 
                 doubleToString(rank) + " " + followings );
  }
};

double PageRankReducer::D = 0.85;
 
int main(int argc, char *argv[]) {
  return HadoopPipes::runTask(HadoopPipes::TemplateFactory<
                              PageRankMapper, 
                              PageRankReducer>() );
}
