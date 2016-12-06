#include <algorithm>
#include <limits>
#include <string>

#include "stdint.h"

#include "Pipes.hh"
#include "TemplateFactory.hh"
#include "StringUtils.hh"

#include "utils.hh"

class InitMapper : public HadoopPipes::Mapper {
public:
  InitMapper( HadoopPipes::TaskContext& context ) {}

  void map( HadoopPipes::MapContext& context ) {
    string line = context.getInputValue();

    vector< string > params =
      HadoopUtils::splitString( line, "\t " );
    
    context.emit( params[1], params[0] );
    context.emit( params[0], "-");
  }
};

class InitCombiner : public HadoopPipes::Reducer {
public:
  InitCombiner( HadoopPipes::TaskContext& context ) {}

  void reduce( HadoopPipes::ReduceContext& context ) {
    
  }
};

class InitReducer : public HadoopPipes::Reducer {
public:
  InitReducer( HadoopPipes::TaskContext& context ) {}

  void reduce( HadoopPipes::ReduceContext& context ) {
    vector< int > followings;

    while ( context.nextValue() ) {
      string value = context.getInputValue();
      if (value != "-") {
        followings.push_back( HadoopUtils::toInt( value ) );
      }
    }

    if ( followings.size() > 0 ) {
      context.emit( context.getInputKey(), "1 " + vectorToString(followings));
    } else {
      context.emit( context.getInputKey(), "1 -");
    }
  }
};
 
int main(int argc, char *argv[]) {
  return HadoopPipes::runTask(HadoopPipes::TemplateFactory<
                              InitMapper, 
                              InitReducer>() );
}