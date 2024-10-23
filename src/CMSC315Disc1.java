import java.util.*;
import java.io.*;

public class CMSC315Disc1 {
  PriorityQueue<String> queueOne = new PriorityQueue<String>();
  PriorityQueue<String> queueTwo = new PriorityQueue<String>();

  public String getUnion() {
    Iterator first = queueOne.iterator();

    String union;

    while (first.hasNext()) {
      Iterator second = queueTwo.iterator();

      String next_one = first.next();
      
      while (second.hasNext) {
        String next_two = second.next();

        if (next_one.equals(next_two)) {
          union.append(" " + next_one);
        }
      }

    }

    return union;
  }

  public String getDifference() {
    Iterator first = queueOne.iterator();
    Iterator second = queueTwo.iterator();

    String union;

    while (first.hasNext()) {
      Iterator comp = queueTwo.iterator();

      String next_one = first.next();
      
      while (comp.hasNext) {
        String next_two = comp.next();

        if (!next_one.equals(comp)) {
          union.append(" " + next_one);
        }
      }

    }

    while (second.hasNext()) {
      Iterator comp = queueOne.iterator();

      String next_two = second.next();

      while (comp.hasNext()) {
        String next_one = comp.next();

        if (!next_two.equals(next_one)) {
          union.append(" " + next_two);
        }
      }
    }

    return union;
  }

  public String getIntersect() {
    return "working";
  }

  public static void main(String[] args) {
    queueOne.add("Giselle");
    queueOne.add("Jamal");
    queueOne.add("John");
    queueOne.add("Soraya");
    queueOne.add("Kevin");
    queueOne.add("Taylor");

    queueTwo.add("Giselle");
    queueTwo.add("Katie");
    queueTwo.add("Kevin");
    queueTwo.add("Michelle");
    queueTwo.add("Ryan");

    System.out.println("The union of the two queues is:" + " " + getUnion());
    System.out.println("The difference in the two queues is:" + " " + getDifference());
    System.out.println("The intersection of the two queues is:" + " " + getIntersect());
}
