import java.util.Collections;
import java.util.List;

/** Implements the quicksort algorithm.
 *   @author Koffman and Wolfgang
 * */

public class QuickSort {

    /** Sort the table using the quicksort algorithm.
     pre: table contains Comparable objects.
     post: table is sorted.
     * @param table The array to be sorted
     */
    public static < Integer extends Comparable < Integer >> void sort(List<Flight> table) {
        // Sort the whole table.
        quickSort(table, 0, table.size() - 1);
    }

    /** Sort a part of the table using the quicksort algorithm.
     post: The part of table from first through last is sorted.
     @param table The list to be sorted
     @param first The index of the low bound
     @param last The index of the high bound
     */
    public static void quickSort(List<Flight> table, int first, int last) {
        if (first < last) { // There is data to be sorted.
            // Partition the table.
            int pivIndex = partition(table, first, last);
            // Sort the left half.
            quickSort(table, first, pivIndex - 1);
            // Sort the right half.
            quickSort(table, pivIndex + 1, last);
        }
    }

    /** Partition the table so that values from first to pivIndex
     are less than or equal to the pivot value, and values from
     pivIndex to last are greater than the pivot value.
     @param table The table to be partitioned
     @param first The index of the low bound
     @param last  The index of the high bound
     @return The location of the pivot value
     */
    private static int partition(List<Flight> table, int first, int last) {
        // Select the first item as the pivot value.
        double pivot =  table.get(first).getPricePerSeat();
        int up = first;
        int down = last;
        do {
      /* Invariant:
         All items in table[first . . . up - 1] <= pivot
         All items in table[down + 1 . . . last] > pivot
       */
            while ( (up < last) && (pivot >= table.get(up).getPricePerSeat())) {
                up++;
            }
            // assert: up equals last or table[up] > pivot.
            while (pivot < table.get(down).getPricePerSeat()) {
                down--;
            }
            // assert: down equals first or table[down] <= pivot.
            if (up < down) { // if up is to the left of down.
                // Exchange table[up] and table[down].
                Collections.swap(table, up, down);
            }
        }
        while (up < down); // Repeat while up is left of down.

        // Exchange table[first] and table[down] thus putting the
        // pivot value where it belongs.
        Collections.swap(table, first, down);

        // Return the index of the pivot value.
        return down;
    }


}

