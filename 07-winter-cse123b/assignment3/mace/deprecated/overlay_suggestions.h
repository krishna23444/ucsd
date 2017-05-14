#define SUGGESTION_NBR_SIZE 1001
struct suggestion_nbr_size {
  mace_key session_id;
  int size;
};

#define SUGGESTION_NBR 1002
#define RANDOM_NEIGHBOR 0
typedef enum {ADD_NEW_NBR, DROP_NBR} SuggestionAction;
struct suggestion_nbr {
  int neighbor_type;
  mace_key session_id;
  mace_key neighbor_id;
  SuggestionAction action;
  double suggestion_period;
}; 
