#include <algorithm>
#include <cstdlib>     
#include <ctime>
#include <functional>
#include <limits>
#include <string>
#include <utility>

#include "stdint.h"

#include "Pipes.hh"
#include "TemplateFactory.hh"
#include "StringUtils.hh"

#include "utils.hh"

class FinalMapper : public HadoopPipes::Mapper {
public:
  FinalMapper( HadoopPipes::TaskContext& context ) {}

  void map( HadoopPipes::MapContext& context ) {
    string line = context.getInputValue();

    vector< string > params =
      HadoopUtils::splitString( line, "\t " );

    srand (time(NULL));
    int key = rand() % 16384;

    context.emit( HadoopUtils::toString(key), 
                  params[0] + " " + params[1] );
  }
};

class FinalCombiner : public HadoopPipes::Reducer {
public:
  FinalCombiner() {}
  FinalCombiner( HadoopPipes::TaskContext& context ) {}

  void emit( HadoopPipes::ReduceContext& context,
             pair< double, int > value) {
    context.emit( "0", HadoopUtils::toString( value.second ) + 
                       " " + doubleToString( value.first ) );
  }

  void reduce( HadoopPipes::ReduceContext& context ) {
    vector< pair< double, int > > values;

    while ( context.nextValue() ) {
      vector< string > value = 
        HadoopUtils::splitString( context.getInputValue(), " " );

      int user = HadoopUtils::toInt(context.getInputKey());
      double rank = HadoopUtils::toFloat(value[0]);

      values.push_back(make_pair( rank, user ));
    }
    
    sort( values.begin(), values.end(), greater< pair< double, int > >() );

    for (unsigned i = 0, lim = 5; i < values.size() && lim > 0; ++i, --lim) {
      this->emit( context, values[i] );
    }
  }
};

class FinalReducer : public FinalCombiner {
public:
  FinalReducer( HadoopPipes::TaskContext& context ) {}

  void emit( HadoopPipes::ReduceContext& context,
             pair< double, int > value ) {
    context.emit( HadoopUtils::toString( value.second ), 
                  doubleToString( value.first ) );
  }
};
 
int main(int argc, char *argv[]) {
  return HadoopPipes::runTask(HadoopPipes::TemplateFactory<
                              FinalMapper, 
                              FinalReducer,
                              void,
                              FinalCombiner>() );
}