
#include <iostream>
#include <string>

using namespace std;

struct cemetery
{
  int security_num = 1000000;
  int is_del = 1;
};

struct whole_cemetery{
  cemetery* my_cemetery;
  int* qnt_per_floor_whole;
  int size;
};


int
hashfunction (int social_num, int num_floor)
{
  int index = social_num % num_floor;
  return index;
}

int
fake_binary_search (int social_num, int index, int caves_per_floor,
		    cemetery arr[], int qnt_per_floor[])
{
  int qnt = qnt_per_floor[index -1];
  int l = ((index - 1) * caves_per_floor);
  int r = l + (qnt -1);

  while (l <= r)
    {
      int m = (int) ((l + r) / 2);
      if (social_num <= arr[m].security_num)
	{
	  r = m - 1;
	}
      else
	{
	  l = m + 1;
	}
    }
  return l;
}

void add_function (int social_num, int caves_per_floor, cemetery arr[], int num_floor, bool should_print, int qnt_per_floor[])
{
    int index = hashfunction(social_num, num_floor);
    for (int i = index*caves_per_floor; i <= num_floor*caves_per_floor; i += caves_per_floor){
        if (i == num_floor*caves_per_floor){
          i = 0;
        }

        int my_floor = i/caves_per_floor;
        if (qnt_per_floor[my_floor] == caves_per_floor){
          continue;
        }
        
        int pos = fake_binary_search(social_num, my_floor + 1, caves_per_floor, arr, qnt_per_floor);
        
        for (int k = (i + qnt_per_floor[my_floor]); k > pos; k--) {
            arr[k] = arr[k - 1];
        }

        arr[pos].security_num = social_num;
        arr[pos].is_del = 1; 
        qnt_per_floor[my_floor] = qnt_per_floor[my_floor] + 1;
        
    
        if (should_print){
          cout << my_floor << "." << pos - i << "\n";
        }
        return;



    }
}

bool find (int social_num, int num_floor, int caves_per_floor, cemetery arr[],int answer[], int qnt_per_floor[]){
 

  int temp_index = 0;

  for (int i = 0; i < num_floor; i ++){
      temp_index = hashfunction (social_num + i, num_floor);
      temp_index++;
      int pos = fake_binary_search (social_num,temp_index, caves_per_floor, arr, qnt_per_floor);
      
      if (arr[pos].security_num == social_num)
        {
          answer[0] = temp_index;
          answer[1] = pos;
          return false;
        }      
  }
  return true;

}




whole_cemetery rehashing (int num_floor, int caves_per_floor, cemetery arr[], int qnt_per_floor[])
{   
    int size = 0;
    whole_cemetery my_entire_cemetery;  
    int new_num_floor = num_floor * 2 + 1;
    cemetery *auxiliary = new cemetery[new_num_floor * caves_per_floor];
    int *qnt_per_floor_auxiliary = new int[new_num_floor];
    
  
    for (int i = 0; i < num_floor * caves_per_floor; i++)
      {
        if (arr[i].is_del != -1){
            size++;
            int social_num = arr[i].security_num;
            add_function(social_num, caves_per_floor, auxiliary, new_num_floor, false, qnt_per_floor_auxiliary);
        }
        
      }

    
    my_entire_cemetery.my_cemetery = auxiliary;
    my_entire_cemetery.qnt_per_floor_whole = qnt_per_floor_auxiliary;
    my_entire_cemetery.size = size;
    
    return my_entire_cemetery;

}




int
main ()
{
  string my_input, num_floor2, caves_per_floor2, command, social_num2;
  int empty_space;

  getline (cin, my_input);
  empty_space = my_input.find (" ");

  num_floor2 = my_input.substr (0, empty_space);
  caves_per_floor2 = my_input.substr (empty_space + 1, my_input.length ());

  int num_floor = stoi (num_floor2);
  int caves_per_floor = stoi (caves_per_floor2);
  cemetery *hash = new cemetery[num_floor * caves_per_floor];

  int *qnt_per_floor = new int[num_floor];

  whole_cemetery my_entire_cemetery;

  int capacity = 0;
  

  while (getline (cin, my_input) && my_input != "END")
    {

      empty_space = my_input.find (" ");
      command = my_input.substr (0, empty_space);
      social_num2 = my_input.substr (empty_space + 1, my_input.length ());
      int social_num = stoi (social_num2);

      if (command == "ADD")
      {
          
        if (capacity == caves_per_floor*num_floor)
          {     
                my_entire_cemetery = rehashing (num_floor, caves_per_floor, hash, qnt_per_floor); 
                hash = my_entire_cemetery.my_cemetery;
                qnt_per_floor = my_entire_cemetery.qnt_per_floor_whole;
                capacity = my_entire_cemetery.size;
                num_floor = num_floor * 2 + 1;

          }
        
      add_function (social_num, caves_per_floor, hash, num_floor, true, qnt_per_floor);
      capacity ++;
      

      }
      else if (command == "QRY")
	{
	  int answer[2];
	  bool a = false;
	  a = find (social_num, num_floor, caves_per_floor, hash, answer, qnt_per_floor);
	  if (a)
	    {
	      cout << "?.?\n";
	    }
	  else
	    {
	      if (hash[answer[1]].is_del == -1)
		{
		  cout << "?.?\n";
		}
	      else
		{
		    
		  cout << answer[0] -1 << "." << (answer[1]) % caves_per_floor << "\n";

		}

	    }
	}
      else if (command == "REM")
	{
	  int answer[2];
	  bool a = false;
	  a = find(social_num, num_floor, caves_per_floor, hash, answer, qnt_per_floor);
	  
	  if (a)
	    {
	      cout << "?.?\n";
	    }
    else
      {
        hash[answer[1]].is_del = -1;  
        cout << answer[0] -1 << "." << (answer[1]) % caves_per_floor << "\n";
      }
	    
	}



    }

  delete[](qnt_per_floor);
  delete[](hash);
  return 0;
}
