public class Util {
  public static boolean sameClass(Class type, Object ... classes) {
    for (Object o : classes)
      if (o.getClass() != type)
        return false;
    return true;
  }

  public static boolean sameClass(Object ... classes) {
    for (int i = 1; i < classes.length; ++i)
      if (classes[i].getClass() != classes[i-1].getClass())
        return false;
    return true;
  }
  
  private static boolean allNumbers(AnyType ... objects) {
    for (AnyType a : objects)
      if (!(a instanceof NumberType))
        return false;
    return true;
  }
  
  public static void promoteInts(AnyType ... args) {
    if (!allNumbers(args))
      return;
  
  // TODO: Error handling
  
    for (int i = 0; i < args.length; i++)
      if (sameClass(IntegerType.class, args[i]))
        args[i] = ((IntegerType)args[i]).toDecimalType();
  }
  
  public static NumberType promoteInt(AnyType a) {
    if (sameClass(IntegerType.class, a))
      return ((IntegerType)a).toDecimalType();
    return (NumberType)a;
  }
}
