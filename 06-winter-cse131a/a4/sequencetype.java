import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;

public class SequenceType extends AnyType {
  private List<AnyType> items;
  
  public SequenceType(AnyType ... data) {
    items = new ArrayList<AnyType>();
    add(data);
  }

  public SequenceType(List<AnyType> items) {
    this.items = new ArrayList<AnyType>(items);
  }

  public boolean isSingleton() {
    return size() == 1;
  }

//  public AnyType toSingleton() throws NonSingletonSequenceException {
//    if (!isSingleton())
//      throw new NonSingletonSequenceException();
//
//    return items.get(0);
//  }
 
  public AnyType last() {
    return items.get(items.size() - 1);
  }

  public List<AnyType> getItems() {
    return items;
  }
  
  public AnyType get(int i) {
    return items.get(i);
  }

  public void setItems(List<AnyType> items) {
    this.items = items;
  }

  public void add(AnyType ... newItems) {
    for (AnyType newItem : newItems) {
      // flatten while inserting children if newItems is a sequence
      if (newItem.getClass() == SequenceType.class) {
        List<AnyType> list = ((SequenceType)newItem).getItems();
        for (AnyType a : list)
          items.add(a);
      } else
        items.add(newItem);
    }
  }

  public int size() {
    return items.size();
  }
  
  public boolean isEmpty() {
    return size() == 0;
  }

  public static SequenceType range(IntegerType start, IntegerType end) {
    SequenceType s = new SequenceType();

    // handle decrementing ranges
    if (start.getValue().compareTo(end.getValue()) > 0)
      for (BigInteger i = start.getValue();  i.compareTo(end.getValue()) >= 0;
           i = i.subtract(new BigInteger("1")))
        s.add(new IntegerType(i));
    // handle incrementing ranges
    else
      for (BigInteger i = start.getValue(); i.compareTo(end.getValue()) <= 0;
           i = i.add(new BigInteger("1")))
        s.add(new IntegerType(i));

    return s;
  }
  
  
  public IntegerType length() {
    return new IntegerType(size());
  }

  public AnyType first() {
    if (size() >= 1)
      return items.get(0);
    
    return new SequenceType();
  }
  
  public SequenceType tail() {
    if (size() >= 2)
      return new SequenceType(items.subList(1, items.size()));

    return new SequenceType();
  }

  public String stringOf() {
    String s = "";

    for (int i = 0; i < items.size(); ++i) {
      s += items.get(i);
      if (i < items.size() - 1)
        s += " ";
    }
    
    return s;
  }

  public boolean equals(Object o) {
    if (!(o instanceof SequenceType))
      return false;

    return super.equals(o) && items.equals(((SequenceType)o).getItems());
  }
  
  public int hashCode() {
    return super.hashCode() + items.hashCode();
  }

  public String toString() {
    return items.toString();
  }
}
