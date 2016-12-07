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

typedef pair< double, int > sortvalue;

class FinalMapper : public HadoopPipes::Mapper {
public:
  FinalMapper( HadoopPipes::TaskContext& context ) {}

  void map( HadoopPipes::MapContext& context ) {
    string line = context.getInputValue();

    vector< string > params =
      HadoopUtils::splitString( line, "\t " );

    context.emit( "0", params[0] + " " + params[1] );
  }
};

class FinalReducer : public HadoopPipes::Reducer {
public:
  FinalReducer( HadoopPipes::TaskContext& context ) {}

  void reduce( HadoopPipes::ReduceContext& context ) {
    vector< sortvalue > values;

    while ( context.nextValue() ) {
      vector< string > value =
        HadoopUtils::splitString( context.getInputValue(), " " );

      int user = HadoopUtils::toInt(value[0]);
      double rank = HadoopUtils::toFloat(value[1]);

      insert( values, make_pair( -rank, user ) );
    }

    sort( values.begin(), values.end(), greater< sortvalue >() );

    for (unsigned i = 0, lim = 5; i < values.size() && lim > 0; ++i, --lim) {
      emit( context, values[i] );
    }
  }

  void insert( vector< sortvalue >& v, sortvalue val ) {
    if (!v.size()) {
      v.push_back(val);
      return;
    }
    vector< sortvalue >::iterator it =
      lower_bound(v.begin(), v.end(), val);
    v.insert(it, val);
    if (v.size() > 5) {
      v.pop_back();
    }
  }

  void emit( HadoopPipes::ReduceContext& context,
             sortvalue value ) {
    context.emit( HadoopUtils::toString( value.second ),
                  doubleToString( -value.first ) );
  }
};

int main(int argc, char *argv[]) {
  return HadoopPipes::runTask(HadoopPipes::TemplateFactory<
                              FinalMapper,
                              FinalReducer>() );
}
