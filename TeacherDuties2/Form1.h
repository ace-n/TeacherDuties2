#pragma once

namespace TeacherDuties2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	// Added
	using namespace System::IO;

	// SQL functionality
	using namespace System::Data;
	using namespace System::Data::SqlTypes;
	using namespace System::Data::SqlClient;
	using namespace System::Configuration;

	public class BuildTeacherDuties
	{
		// Placeholder array (Mr. Smith, delete this!)
		public: static array< String^ > ^ placeHolder(void)
		{
			array< String^ > ^ retArr = { "Adams", "Simon", "Fletcher", "Thomas", "Fife", "Stevens", "Timms" };
			return retArr;
		}

		public: static array< String^ > ^ dutyDatePlaceHolder(void)
		{
			array< String^ > ^ retArr = { "11/8-12", "11/1-5", "1/17-21", "12/6-10", "2/7-11", "1/24-28", "11/15-19"};
			return retArr;
		}
			
		public: static array< String^ > ^ firstSecondPlaceHolder(void)
		{
			array< String^ > ^ retArr = { "first", "first", "second", "first", "second", "second", "first"};
			return retArr;
		}
			

		// Calculate the number of lunch periods required for both the cafeteria and snack bar
		// & after a variable type denotes it is "ByRef" (referenced - pointer to output value) as opposed to "ByVal" (input only value)

		// INPUT: None
		// ACTIONS: Gets a list of duty dates where each date occurs only once
		// OUTPUT: The total number of duty dates for both the cafeteria and snack bar

	    public: static void calculateNumberOfDays(int& cafeteria, int& snackBar)
		{

			// Step 1: Get the SQL data 
			array< String^ > ^ dutyDateArray = dutyDatePlaceHolder(); // "DutyDate" field of the "LunchDutyDays" database as an array of strings

			// Step 2: Convert the SQL data to the total number of duty dates (1 duty date = 5 actual days)
			cafeteria = dutyDateArray->Length;
			snackBar = dutyDateArray->Length;
		}

		// Calculate the number of teachers who have first lunch (using the data from the database)
	    public: static int calculateNumberOfTeachersFirstLunch(void)
		{

			// Get database array [Mr. Smith: insert proper code here]
			array< String^ > ^ dbLunchtimeArray = firstSecondPlaceHolder(); // = contents of "TeacherLunchPeriod" field of "Teachers" database

			// Parse through the read data and count how many teachers have first lunch
			int firstLunchCnt = 0;
			for each ( System::String^ text in dbLunchtimeArray ) {

				// If the "TeacherLunchPeriod" field is "first", then add one to the number of teachers with first lunch
				if (text->ToLower() == "first")
				{
					firstLunchCnt++;
				}
			}

			// Return the number of teachers with first lunch
			return firstLunchCnt;
		}

		// Calculate the number of teachers who have second lunch
		// This code is only slightly modified from that used to calculate the data for first lunch
	    public: static int calculateNumberOfTeachersSecondLunch(void)
		{

			// Get database array [Mr. Smith: insert proper code here]
			array< String^ > ^ dbLunchtimeArray = firstSecondPlaceHolder(); // = contents of "TeacherLunchPeriod" field of "Teachers" database

			// Parse through the read data and count how many teachers have second lunch
			int secondLunchCnt = 0;
			for each ( System::String^ text in dbLunchtimeArray ) {

				// If the "TeacherLunchPeriod" field is "first", then add one to the number of teachers with second lunch
				if (text->ToLower() == "second")
				{
					secondLunchCnt++;
				}
			}

			// Return the number of teachers with first lunch
			return secondLunchCnt;

		}

		// INPUT: number of duty weeks in cafeteria and snackbar, number of duty weeks required for first / second lunch
		// ACTIONS: Organize the duties for each teacher

		// NOTE: There are FOUR duties per teacher - this must use them all!

	    public: static void TeachersToDates(int cafeteria, int snackBar, int firstLunch, int secondLunch)
		{

			// ----- ROUTINE A: Get two lists of teachers (one for each lunch period) -----

			// Step A: Get a list of all teachers and lunch periods
			array < String^ > ^ ttdEntireRosterArray = placeHolder(); // = "TeacherName" field of "Teachers" database
			array < String^ > ^ ttdLunchesArray = firstSecondPlaceHolder(); // = "TeacherLunchPeriod" field of "Teachers" database

			// Step B: Create required lists
			array < String^ > ^ ttdFirstLunchTeachers = gcnew array < String^ > (firstLunch-1);
			array < String^ > ^ ttdSecondLunchTeachers = gcnew array < String^ > (secondLunch-1);

			// Step C: Sort the teachers into two lists based on their lunch period
			//int firstLunchCnt = 0;
			//int secondLunchCnt = 0;
			//for (int i = 0; i < ttdEntireRosterArray->Length; i++)
			//{
			//	String^ lunchPeriod = Convert::ToString( ttdLunchesArray->GetValue(i) ) -> ToLower();
			//	String^ teacherName = Convert::ToString( ttdEntireRosterArray->GetValue(i) );
			//	if (lunchPeriod = "first")
			//	{
			//		// Add item to array
			//		ttdFirstLunchTeachers->SetValue(teacherName, firstLunchCnt);
			//	}
			//	else
			//	{
			//		// Add item to array
			//		ttdSecondLunchTeachers->SetValue(teacherName, secondLunchCnt);
			//	}
			//}


			// ROUTINE B: Get duty date list and construct four copies of it
			array < String ^ > ^ ttdBaseDutyDates = dutyDatePlaceHolder();  // DutyDate field of LunchDutyDays database
			array < String ^ > ^ ttdCafOneDutyDates = (array < String ^ > ^)ttdBaseDutyDates->Clone();
			array < String ^ > ^ ttdCafTwoDutyDates = (array < String ^ > ^)ttdCafOneDutyDates->Clone();
			array < String ^ > ^ ttdSnackOneDutyDates = (array < String ^ > ^)ttdCafOneDutyDates->Clone();
			array < String ^ > ^ ttdSnackTwoDutyDates = (array < String ^ > ^)ttdCafOneDutyDates->Clone();


			// -------------- ROUTINE C: Pair the teachers with their dates --------------

			// Step 1: Figure out how many duties each teacher must take as a minimum
			int firstLunchDutiesPerTeacher = Math::Ceiling((cafeteria+snackBar)/firstLunch);
			int secondLunchDutiesPerTeacher = Math::Ceiling((cafeteria+snackBar)/secondLunch);

			// Step 2: Make sure that there are no more than three duties per teacher (otherwise, warn the user)
			if ((firstLunchDutiesPerTeacher > 3) | (secondLunchDutiesPerTeacher > 3))
			{
				MessageBox::Show("TeachersToDates: ALERT! Some teachers may be required to have more than three duties.");
			}

			// Step 2: Create variables for first lunch data handling
			// NOTE: This uses jagged arrays to permit more than three duties to be assigned to teachers
			array < array < String ^ > ^ > ^ ttdFirstLunchTeacherDutyStack = gcnew array < array< String^ > ^ > (firstLunch);
			for (int i = 0; i < firstLunch; i++ )
			{
				ttdFirstLunchTeacherDutyStack->SetValue(gcnew array< String^ >(firstLunchDutiesPerTeacher),i);
			}

			// Step 3: Shallow copy jagged array from above into another jagged array (this one for second lunch duties)
			array < array < String ^ > ^ > ^ ttdSecondLunchTeacherDutyStack = (array < array < String ^ > ^ > ^)ttdFirstLunchTeacherDutyStack->Clone();

			// -------------- ROUTINE D: Assign first lunch teacher duties --------------
			Random^ randGen = gcnew Random;
			for(int i = 0; i < firstLunch; i++)
			{
				// The above loop iterates through the teachers

				// Step A: Take two duties from the longest two lists, and one from the shortest two
				int max = Math::Max(ttdCafOneDutyDates->Length+ttdCafTwoDutyDates->Length,ttdSnackOneDutyDates->Length+ttdSnackTwoDutyDates->Length);
				if (max = (ttdSnackOneDutyDates->Length + ttdSnackTwoDutyDates->Length))
				{
					// This section is triggered if the snack bar arrays are longer than the cafeteria arrays

					// Variables
					int randIntA = 0;
					int randIntB = 0;
					int randIntC = 0;
					String^ randDateA = "none";
					String^ randDateB = "none";
					String^ randDateC = "none";

					// Action A: Pick three random duty dates from each array that do not coincide with each other.
					while (true)
					{
						// Get random integers
						randIntA = randGen->Next(0,ttdSnackOneDutyDates->Length);
						randIntB = randGen->Next(0,ttdSnackTwoDutyDates->Length);
						randIntC = randGen->Next(0,ttdCafOneDutyDates->Length);

						// Set placeholder/default values
						randDateA = "none1";
						randDateB = "none2";
						randDateC = "none3";

						// Get data at indices referenced by above random integers
						// Add an S or a C to indicate whether date is for snackbar or cafeteria
						if (ttdSnackOneDutyDates->Length < 0)
						{
							randDateA=String::Concat("S", Convert::ToString(ttdSnackOneDutyDates->GetValue(randIntA)));
						}
						if (ttdSnackTwoDutyDates->Length < 0)
						{
							randDateB=String::Concat("S", Convert::ToString(ttdSnackTwoDutyDates->GetValue(randIntB)));
						}
						if (ttdCafOneDutyDates->Length < 0)
						{
							randDateC=String::Concat("C", Convert::ToString(ttdCafOneDutyDates->GetValue(randIntC)));
						}

						// Check to see if above data are equal (if they are "none", the duty date array they take elements from was nil)
						if (randDateA != randDateB & randDateA != randDateC & randDateB != randDateC)
						{
							// If all of the above data are different, do the following

							// Remove above data from each of their arrays
							ttdSnackOneDutyDates = removeElement(randIntA, ttdSnackOneDutyDates);
							ttdSnackTwoDutyDates = removeElement(randIntB, ttdSnackTwoDutyDates);
							ttdCafOneDutyDates = removeElement(randIntC, ttdCafOneDutyDates);

							// [Mr. Smith] Set each duty date's respective UsedFlags to TRUE
							// Possible method
							// Step 1: Find location of current duty date in entire database
							// Step 2: Set the corresponding flag to TRUE
							// NOTE: There are three duty dates flying around here (2 snack bar, 1 cafeteria)
							//	     randDateA / randDateB: Snackbar; randDateC: cafeteria



							// If all of above data are different, leave the while loop
							break;
						}


					}

					// Action B: Add the current duty dates to the first lunch duty stack
					ttdFirstLunchTeacherDutyStack->SetValue(randDateA,i,0);
					ttdFirstLunchTeacherDutyStack->SetValue(randDateB,i,1);
					ttdFirstLunchTeacherDutyStack->SetValue(randDateC,i,2);

				}
				else
				{
					// This section is triggered if the snack bar arrays are shorter or the same size than the cafeteria arrays

					// Variables
					int randIntA = 0;
					int randIntB = 0;
					int randIntC = 0;
					String^ randDateA = "none";
					String^ randDateB = "none";
					String^ randDateC = "none";

					// Action A: Pick three random duty dates from each array that do not coincide with each other.
					while (true)
					{
						// Get random integers
						randIntA = randGen->Next(0,ttdSnackOneDutyDates->Length);
						randIntB = randGen->Next(0,ttdCafOneDutyDates->Length);
						randIntC = randGen->Next(0,ttdCafTwoDutyDates->Length);

						// Set placeholder/default values
						randDateA = "none1";
						randDateB = "none2";
						randDateC = "none3";

						// Get data at indices referenced by above random integers
						// Add an S or a C to indicate whether date is for snackbar or cafeteria
						if (ttdSnackOneDutyDates->Length < 0)
						{
							randDateA=String::Concat("S", Convert::ToString(ttdSnackOneDutyDates->GetValue(randIntA)));
						}
						if (ttdCafOneDutyDates->Length < 0)
						{
							randDateB=String::Concat("C", Convert::ToString(ttdCafOneDutyDates->GetValue(randIntB)));
						}
						if (ttdCafTwoDutyDates->Length < 0)
						{
							randDateC=String::Concat("C", Convert::ToString(ttdCafTwoDutyDates->GetValue(randIntC)));
						}

						// Check to see if above data are equal (if they are "none", the duty date array they take elements from was nil)
						if (randDateA != randDateB & randDateA != randDateC & randDateB != randDateC)
						{
							// If all of the above data are different, do the following

							// Remove above data from each of their arrays
							ttdSnackOneDutyDates = removeElement(randIntA, ttdSnackOneDutyDates);
							ttdCafOneDutyDates = removeElement(randIntB, ttdCafOneDutyDates);
							ttdCafTwoDutyDates = removeElement(randIntC, ttdCafTwoDutyDates);

							// [Mr. Smith] Set each duty date's respective UsedFlags to TRUE
							// Possible method
							// Step 1: Find location of current duty date in entire database
							// Step 2: Set the corresponding flag to TRUE
							// NOTE: There are three duty dates flying around here (1 snack bar, 2 cafeteria)
							//	     randDateB / randDateC: cafeteria; randDateA: snackbar

							// If all of above data are different, leave the while loop
							break;
						}


					}

					// Action B: Assign the current teacher the random duty dates selected in Action A
					
					// Note: these are all in the Teachers table
					// Set ith value of "SnackbarDutyOne" to "randDateA"
					// Set ith value of "SnackbarDutyTwo" to "randDateB"
					// Set ith value of "CafeteriaDutyOne" to "randDateC"

					ttdFirstLunchTeacherDutyStack->SetValue(randDateA,i,0);
					ttdFirstLunchTeacherDutyStack->SetValue(randDateB,i,1);
					ttdFirstLunchTeacherDutyStack->SetValue(randDateC,i,2);

				}
			}

			// ----- ROUTINE E: Handle second lunch duties -----

			// Create copies of duty arrays
			ttdCafOneDutyDates = (array < String ^ > ^)ttdBaseDutyDates->Clone();
			ttdCafTwoDutyDates = (array < String ^ > ^)ttdBaseDutyDates->Clone();
			ttdSnackOneDutyDates = (array < String ^ > ^)ttdBaseDutyDates->Clone();
			ttdSnackTwoDutyDates = (array < String ^ > ^)ttdBaseDutyDates->Clone();

			for(int i = 0; i < secondLunch; i++)
			{
				// The above loop iterates through the teachers

				// Step B: Take two duties from the longest two lists, and one from the shortest two
				int max = Math::Max(ttdCafOneDutyDates->Length+ttdCafTwoDutyDates->Length,ttdSnackOneDutyDates->Length+ttdSnackTwoDutyDates->Length);
				if (max = (ttdSnackOneDutyDates->Length+ttdSnackTwoDutyDates->Length))
				{
					// This section is triggered if the snack bar arrays are longer than the cafeteria arrays

					// Variables
					int randIntA = 0;
					int randIntB = 0;
					int randIntC = 0;
					String^ randDateA = "none";
					String^ randDateB = "none";
					String^ randDateC = "none";

					// Action A: Pick three random duty dates from each array that do not coincide with each other.
					while (true)
					{
						// Get random integers
						randIntA = randGen->Next(0,ttdSnackOneDutyDates->Length);
						randIntB = randGen->Next(0,ttdSnackTwoDutyDates->Length);
						randIntC = randGen->Next(0,ttdCafOneDutyDates->Length);

						// Set placeholder/default values
						randDateA = "none1";
						randDateB = "none2";
						randDateC = "none3";

						// Get data at indices referenced by above random integers
						if (ttdSnackOneDutyDates->Length < 0)
						{
							randDateA=String::Concat("S", Convert::ToString(ttdSnackOneDutyDates->GetValue(randIntA)));
						}
						if (ttdSnackTwoDutyDates->Length < 0)
						{
							randDateB=String::Concat("S", Convert::ToString(ttdSnackTwoDutyDates->GetValue(randIntB)));
						}
						if (ttdCafOneDutyDates->Length < 0)
						{
							randDateC=String::Concat("C", Convert::ToString(ttdCafOneDutyDates->GetValue(randIntC)));
						}

						// Check to see if above data are equal (if they are "none", the duty date array they take elements from was nil)
						if (randDateA != randDateB & randDateA != randDateC & randDateB != randDateC)
						{
							// If all of the above data are different, do the following

							// Remove above data from each of their arrays
							ttdSnackOneDutyDates = removeElement(randIntA, ttdSnackOneDutyDates);
							ttdSnackTwoDutyDates = removeElement(randIntB, ttdSnackTwoDutyDates);
							ttdCafOneDutyDates = removeElement(randIntC, ttdCafOneDutyDates);

							// [Mr. Smith] Set each duty date's respective UsedFlags to TRUE
							// Possible method
							// Step 1: Find location of current duty date in entire database
							// Step 2: Set the corresponding flag to TRUE
							// NOTE: There are three duty dates flying around here (2 snack bar, 1 cafeteria)
							//	     randDateA / randDateB: Snackbar; randDateC: cafeteria



							// If all of above data are different, leave the while loop
							break;
						}


					}

					// Action B: Assign the current teacher the random duty dates selected in Action A
					ttdSecondLunchTeacherDutyStack->SetValue(randDateA,i,0);
					ttdSecondLunchTeacherDutyStack->SetValue(randDateB,i,1);
					ttdSecondLunchTeacherDutyStack->SetValue(randDateC,i,2);


					// Note: these are all in the Teachers table
					// Set ith value of "SnackbarDutyOne" to "randDateA"
					// Set ith value of "SnackbarDutyTwo" to "randDateB"
					// Set ith value of "CafeteriaDutyOne" to "randDateC"

				}
				else
				{
					// This section is triggered if the snack bar arrays are shorter or the same size than the cafeteria arrays

					// Variables
					int randIntA = 0;
					int randIntB = 0;
					int randIntC = 0;
					String^ randDateA = "none";
					String^ randDateB = "none";
					String^ randDateC = "none";

					// Action A: Pick three random duty dates from each array that do not coincide with each other.
					while (true)
					{
						// Get random integers
						randIntA = randGen->Next(0,ttdSnackOneDutyDates->Length);
						randIntB = randGen->Next(0,ttdCafOneDutyDates->Length);
						randIntC = randGen->Next(0,ttdCafTwoDutyDates->Length);

						// Set placeholder/default values
						randDateA = "none1";
						randDateB = "none2";
						randDateC = "none3";

						// Get data at indices referenced by above random integers
						if (ttdSnackOneDutyDates->Length < 0)
						{
							randDateA=Convert::ToString(ttdSnackOneDutyDates->GetValue(randIntA));
						}
						if (ttdCafOneDutyDates->Length < 0)
						{
							randDateB=Convert::ToString(ttdCafOneDutyDates->GetValue(randIntB));
						}
						if (ttdCafOneDutyDates->Length < 0)
						{
							randDateC=Convert::ToString(ttdCafTwoDutyDates->GetValue(randIntC));
						}

						// Check to see if above data are equal (if they are "none", the duty date array they take elements from was nil)
						if (randDateA != randDateB & randDateA != randDateC & randDateB != randDateC)
						{
							// If all of the above data are different, do the following

							// Remove above data from each of their arrays
							ttdSnackOneDutyDates = removeElement(randIntA, ttdSnackOneDutyDates);
							ttdCafOneDutyDates = removeElement(randIntB, ttdCafOneDutyDates);
							ttdCafTwoDutyDates = removeElement(randIntC, ttdCafTwoDutyDates);

							// [Mr. Smith] Set each duty date's respective UsedFlags to TRUE
							// Possible method
							// Step 1: Find location of current duty date in entire database
							// Step 2: Set the corresponding flag to TRUE
							// NOTE: There are three duty dates flying around here (1 snack bar, 2 cafeteria)
							//	     randDateB / randDateC: cafeteria; randDateA: snackbar

							// If all of above data are different, leave the while loop
							break;
						}


					}

					// Action B: Assign the current teacher the random duty dates selected in Action A
					
					ttdSecondLunchTeacherDutyStack->SetValue(randDateA,i,0);
					ttdSecondLunchTeacherDutyStack->SetValue(randDateB,i,1);
					ttdSecondLunchTeacherDutyStack->SetValue(randDateC,i,2);

					// Note: these are all in the Teachers table
					// Set ith value of "SnackbarDutyOne" to "randDateA"
					// Set ith value of "SnackbarDutyTwo" to "randDateB"
					// Set ith value of "CafeteriaDutyOne" to "randDateC"

				}
			}

			// ------ ROUTINE F: Pair the teacher names with their duties
			
			// Compare lunches/teachers using two arrays requisitioned at the start of the function
			int firstLunchCntr = 0;
			int secondLunchCntr = 0;
			for (int i = 0; i < ttdEntireRosterArray->Length; i++ )
			{
				// Step A: Get lunch period for current teacher
				String^ lunchPeriod = Convert::ToString(ttdLunchesArray->GetValue(i))->ToLower();

				// Step B: Select the next available duty set from the appropriate duty stack
				String^ dutyA = "";
				String^ dutyB = "";
				String^ dutyC = "";

				// Step C: Assign duties from proper source based on lunch period
				if (lunchPeriod = "first")
				{
					// Assign first lunch duties
					dutyA = Convert::ToString(ttdFirstLunchTeacherDutyStack->GetValue(i,0))->ToLower();
					dutyB = Convert::ToString(ttdFirstLunchTeacherDutyStack->GetValue(i,1))->ToLower();
					dutyC = Convert::ToString(ttdFirstLunchTeacherDutyStack->GetValue(i,2))->ToLower();

					// Add to counter
					firstLunchCntr++;

				}
				else
				{
					// Assign first lunch duties
					dutyA = Convert::ToString(ttdSecondLunchTeacherDutyStack->GetValue(i,0))->ToLower();
					dutyB = Convert::ToString(ttdSecondLunchTeacherDutyStack->GetValue(i,1))->ToLower();
					dutyC = Convert::ToString(ttdSecondLunchTeacherDutyStack->GetValue(i,2))->ToLower();

					// Add to counter
					secondLunchCntr++;

				}

				// Step D: Get type of duty date ("c" - cafeteria, "s" - snackbar)
				// dutyA. dutyB, and dutyC all start with either "c" or "s" to signify which duty they are
				String^ dutyDateTypeA = dutyA->Substring(0,1);
				String^ dutyDateTypeB = dutyB->Substring(0,1);
				String^ dutyDateTypeC = dutyC->Substring(0,1);

				// Step E: Assign the current teacher the selected duty stack
				int cafeteriaDutyCnt = 0;
				int snackBarDutyCnt = 0;

				// Step F: Handle duty date A
				if (dutyA->StartsWith("c"))
				{
					// Duty Date A is a cafeteria duty
					
					// Assign teacher the duty date
					// Set ith row in "CafeteriaDutyOne" column of Teachers database to "dutyA->Substring(1)"

					cafeteriaDutyCnt++;
				}
				else if (dutyA->StartsWith("s"))
				{
					// Duty Date A is a snackbar duty

					// Assign teacher the duty date
					// Set ith row in "SnackBarDutyOne" column of Teachers database to "dutyA->Substring(1)"

					snackBarDutyCnt++;
				}

				// Step G: Handle duty date B
				if (dutyB->StartsWith("c"))
				{
					// Duty Date B is a cafeteria duty

					// Assign teacher the duty date, depending on the already-assigned date
					if (cafeteriaDutyCnt = 1)
					{
						// Set ith row in "CafeteriaDutyTwo" column of Teachers database to "dutyB->Substring(1)"
					}
					else
					{
						// Set ith row in "CafeteriaDutyOne" column of Teachers database to "dutyB->Substring(1)"
					}
					
					cafeteriaDutyCnt++;
				}
				else if (dutyB->StartsWith("s"))
				{
					// Duty Date B is a snackbar duty
					
					// Assign teacher the duty date, depending on the already-assigned date
					if (snackBarDutyCnt = 1)
					{
						// Set ith row in "SnackBarDutyTwo" column of Teachers database to "dutyB->Substring(1)"
					}
					else
					{
						// Set ith row in "SnackBarDutyOne" column of Teachers database to "dutyB->Substring(1)"
					}
					
					snackBarDutyCnt++;
				}

				// Step H: Handle duty date C
				if (dutyC->StartsWith("c"))
				{
					// Duty Date B is a cafeteria duty

					// Assign teacher the duty date, depending on the already-assigned dates
					if (cafeteriaDutyCnt = 1)
					{
						// Set ith row in "CafeteriaDutyTwo" column of Teachers database to "dutyC->Substring(1)"
					}
					else if (cafeteriaDutyCnt = 0)
					{
						// Set ith row in "CafeteriaDutyOne" column of Teachers database to "dutyC->Substring(1)"
					}
					else
					{
						// There are three duties of the same type - alert the user that something is wrong
						MessageBox::Show("ERROR! BuildTeacherDuties::TeachersToDates: Three duties are of the same type! This is a critical error!");
					}

					cafeteriaDutyCnt++;
				}
				else if (dutyC->StartsWith("s"))
				{
					// Duty Date C is a snackbar duty
					
					// Assign teacher the duty date, depending on the already-assigned date
					if (snackBarDutyCnt = 1)
					{
						// Set ith row in "SnackBarDutyTwo" column of Teachers database to "dutyC->Substring(1)"
					}
					else if (snackBarDutyCnt = 0)
					{
						// Set ith row in "SnackBarDutyOne" column of Teachers database to "dutyC->Substring(1)"
					}
					else
					{
						// There are three duties of the same type - alert the user that something is wrong
						MessageBox::Show("ERROR! BuildTeacherDuties::TeachersToDates: Three duties are of the same type! This is a critical error!");
					}
					
					snackBarDutyCnt++;
				}

			}
		}

		

		// Removes a value at a given index from an array, and resizes the array accordingly
		public: static array< String^ > ^ removeElement(int elementIndex, array< String^ > ^ arrayStart)
		{
			// Create new array
			array< String^ >^ arrayEnd = gcnew array< String^ >(arrayStart->Length-1);

			// Transfer values
			int j = 0;
			for (int i = 0; i < arrayStart->Length; i++ )
			{
				// If the current element isn't the one being removed, add it to the target return array
				if (i != elementIndex)
				{
					arrayEnd->SetValue(arrayStart->GetValue(i),j);
					j++;
				}
			}

			// Return the new array
			return arrayEnd;
		}

		// Output data to CSV file
		// INPUT: The filepath to the output location
		// ACTIONS:
		// OUTPUT:
		public: static void outputDataToFile(System::String^ filePath)
		{
			// Prompt user as to sorting method (none/by name/by date)
			DialogResult msgResponse = MessageBox::Show("What sorting method do you want to use? YES = Names, NO = Dates, CANCEL = None", "TeacherDuties", MessageBoxButtons::YesNoCancel);

			// Get all entries from teachers database (excluding teacher lunch period)
			array < String^ > ^ tchNameArray = { "Adam", "Johnny", "Brad", "Aessa", "Hannah", "William", "Bob" }; // Array of teacher names from database
			array < String^ > ^ tchCafOne = {"11/8-12", "11/1-5", "1/17-21", "12/6-10", "2/7-11", "1/24-28", "11/15-19"}; // Array of first cafeteria duty dates
			array < String^ > ^ tchCafTwo = {"11/8-12", "11/1-5", "1/17-21", "12/6-10", "2/7-11", "1/24-28", "11/15-19"}; // Array of second cafeteria duty dates
			array < String^ > ^ tchSnakOne = {"11/8-12", "11/1-5", "1/17-21", "12/6-10", "2/7-11", "1/24-28", "11/15-19"}; // Array of first snack bar duty dates
			array < String^ > ^ tchSnakTwo = {"11/8-12", "11/1-5", "1/17-21", "12/6-10", "2/7-11", "1/24-28", "11/15-19"}; // Array of second snack bar duty dates

			// List of lines to be wrote
			array < String^ > ^ writeLineStack = gcnew array < String^ >(tchNameArray->Length);

			// Ready the text-writing objects [PROBLEM]
			StreamWriter^ writer = gcnew StreamWriter( filePath );

			// Sort them accordingly
			int tick = -1;
			if (msgResponse.Equals(DialogResult::Yes))
			{
				// Sort by names

				// Step 1: Sort names alphabetically
				array< String^ > ^ tchNameArraySorted = gcnew array< String^ >(tchNameArray->Length);
				tchNameArray->CopyTo(tchNameArraySorted,0);

				Array::Sort( tchNameArraySorted );

				// Step 2: Sort other data relative to names

				// Scratchpad arrays
				array < String^ > ^ tchCafOneSorted = gcnew array < String^ > (tchNameArray->Length);
				array < String^ > ^ tchCafTwoSorted = tchCafOneSorted;
				array < String^ > ^ tchSnakOneSorted = tchCafOneSorted;
				array < String^ > ^ tchSnakTwoSorted = tchCafOneSorted;

				// Carry out sorting using scratchpad arrays
				int i;
				for ( i = 0; i < tchNameArraySorted->Length; i++)
				{
					// Get current name value
					String^ nameA = Convert::ToString(tchNameArraySorted->GetValue(i));

					// Get index of current name in unsorted array
					int index = Array::IndexOf( tchNameArray, nameA );

					// Transfer the values for the current name in the unsorted arrays to the sorted ones
					tchCafOneSorted->SetValue(tchCafOne->GetValue(index), i);
					tchCafTwoSorted->SetValue(tchCafTwo->GetValue(index), i);
					tchSnakOneSorted->SetValue(tchSnakOne->GetValue(index), i);
					tchSnakTwoSorted->SetValue(tchSnakTwo->GetValue(index), i);

				}

				// Write to the file
				for (i = 0; i < tchNameArray->Length; i++)
				{
					// Create CSV line from various data sources
					String^ lineStr = System::String::Concat(tchNameArraySorted->GetValue(i), ",", tchCafOneSorted->GetValue(i), " / ", tchCafTwoSorted->GetValue(i), ",");
					lineStr = System::String::Concat(lineStr, tchSnakOneSorted->GetValue(i), " / ", tchSnakTwoSorted->GetValue(i));
						
					MessageBox::Show(lineStr);

					// Write the line to a file
					writer->WriteLine(lineStr);
				}

			}
			else if (msgResponse.Equals(DialogResult::No))
			{
				// Step 1: Get a list of dates
				array< String^ > ^ dateArray = placeHolder(); // "DutyDate" field of "LunchDutyDays" table

				// Step 2: Assign each duty date a numerical identifier
				array< int > ^ dateNumArray = gcnew array< int >(dateArray->Length);
				for (int i = 0; i < dateArray->Length; i++ )
				{
					String ^ dateStr = Convert::ToString(dateArray->GetValue(i));
					dateNumArray->SetValue(dateToIntNum(dateStr),i);
				}

				// Step 3: Sort the numerical identifiers
				Array::Sort(dateNumArray);

				// Step 4: Sort data
				array< String^ > ^ fileStringArray = gcnew array < String^ > (dateArray->Length);	// Array of lines to be written to the file
				for (int i = 0; i < dateArray->Length; i++ )
				{
					// Get starting data
					String^ dateStr = Convert::ToString(dateArray->GetValue(i));
					int dateNum = dateToIntNum(dateStr);

					// Get position of unsorted duty date
					int dateSortedPos = Array::IndexOf(dateNumArray,dateNum);

					// Get other unsorted data
					String^ tchCafDutyOne = Convert::ToString(tchCafOne->GetValue(dateSortedPos));
					String^ tchCafDutyTwo = Convert::ToString(tchCafTwo->GetValue(dateSortedPos));
					String^ tchSnakDutyOne = Convert::ToString(tchSnakOne->GetValue(dateSortedPos));
					String^ tchSnakDutyTwo = Convert::ToString(tchSnakTwo->GetValue(dateSortedPos));

					// Add the write-line to fileStringArray
					fileStringArray->SetValue(String::Concat( dateStr,",", tchCafDutyOne,",", tchCafDutyTwo,",", tchSnakDutyOne,",", tchSnakDutyTwo ), dateSortedPos);
						
				}

				// Step 5: Write data to file
				for each (String^ lineStr in fileStringArray)
				{
					writer->WriteLine(lineStr);
				}
			}
			else
			{
				// Just write to the file in database order
				int i = 0;
				for (i = 0; i < tchNameArray->Length; i++)
				{
					// Create CSV line from various data sources
					String^ lineStr = System::String::Concat(tchNameArray->GetValue(i), ",", tchCafOne->GetValue(i), " / ", tchCafTwo->GetValue(i), ",");
					lineStr = System::String::Concat(lineStr, tchSnakOne->GetValue(i), " / ", tchSnakTwo->GetValue(i));

					// Write the line to a file
					writer->WriteLine(lineStr);
				}
			}

			// Close the writing object
			writer->Close();

		}

		// Converts dates to numerical identifiers for sorting purposes (these need not be convertible back to dates)
		// NOTE: July - December are presumed to be in the current year
		// NOTE: By the same token, January - June are presumed to be in the next year
		public: static int dateToIntNum( String^ date )
		{
			// Step 1: Remove second half of date (if one exists)
			// EX: "11/1 - 11/5" --> "11/1"
			if (date->Contains("-"))
			{
				// Remove the second part of the date
				date = date->Substring(0,date->IndexOf("-"));

				// Remove spaces
				date = date->Replace(" ","");
			}

			// Step 2: Get the integer forms of the month and day
			String^ monthStr = date->Substring(0,date->IndexOf("/"));
			String^ dayStr = date->Substring(date->IndexOf("/")+1);

			// Step 3: Convert from month/day to approximate number of days
			int dateInt = int::Parse(monthStr) * 32 + int::Parse(dayStr);
			
			// Step 4: If the date is in the next year, add 365 days to it
			if (int::Parse(monthStr) < 7)
			{
				dateInt += 365;
			}

			// Step 5: Return the final date
			return dateInt;
		}

		// Randomize an array
		public: static array< String^ > ^ randomizeArray( array< String^ > ^ startArray )
		{
			// Create new array and random generator
			array < String^ > ^ randomArray = gcnew array< String^ >(startArray->Length);
			Random^ randGen = gcnew Random;

			// Randomize
			while (startArray->Length > 0)
			{
				// Step A: Get date at the end of the to-be-randomized array
				String^ endTeacher = Convert::ToString(startArray->GetValue(startArray->Length-1));

				// Step B: Remove the current item from the to-be-randomized date array
				Array::Resize(startArray,startArray->Length-1);

				// Step C: Find an empty spot for the current value in the randomized array
				while (true)
				{
					// Get random index
					int index = randGen->Next(0,startArray->Length);

					// If index is empty, add the current date in that index and exit the while loop
					if (String::IsNullOrEmpty(Convert::ToString(startArray->GetValue(index))))
					{
						randomArray->SetValue(endTeacher, index);
						break;
					}
				}
			}
		}

	};

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnCalculateTeacherDuties;
	private: System::Windows::Forms::TextBox^  txtboxCalculatedOutputPath;
	protected: 

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btnGetDutyDateArray;
	private: System::Windows::Forms::TextBox^  txtboxLunchDutyDaysList;
	private: System::Windows::Forms::Label^  lblLunchDutyDaysList;
	private: System::Windows::Forms::Button^  btnGetTeacherList;
	private: System::Windows::Forms::TextBox^  txtboxTeacherListPath;
	private: System::Windows::Forms::Label^  lblTeacherListPath;






	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnCalculateTeacherDuties = (gcnew System::Windows::Forms::Button());
			this->txtboxCalculatedOutputPath = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnGetDutyDateArray = (gcnew System::Windows::Forms::Button());
			this->txtboxLunchDutyDaysList = (gcnew System::Windows::Forms::TextBox());
			this->lblLunchDutyDaysList = (gcnew System::Windows::Forms::Label());
			this->btnGetTeacherList = (gcnew System::Windows::Forms::Button());
			this->txtboxTeacherListPath = (gcnew System::Windows::Forms::TextBox());
			this->lblTeacherListPath = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnCalculateTeacherDuties
			// 
			this->btnCalculateTeacherDuties->Location = System::Drawing::Point(530, 78);
			this->btnCalculateTeacherDuties->Name = L"btnCalculateTeacherDuties";
			this->btnCalculateTeacherDuties->Size = System::Drawing::Size(146, 23);
			this->btnCalculateTeacherDuties->TabIndex = 17;
			this->btnCalculateTeacherDuties->Text = L"Calculate Teacher Duties";
			this->btnCalculateTeacherDuties->UseVisualStyleBackColor = true;
			this->btnCalculateTeacherDuties->Click += gcnew System::EventHandler(this, &Form1::btnCalculateTeacherDuties_Click);
			// 
			// txtboxCalculatedOutputPath
			// 
			this->txtboxCalculatedOutputPath->Location = System::Drawing::Point(154, 80);
			this->txtboxCalculatedOutputPath->Name = L"txtboxCalculatedOutputPath";
			this->txtboxCalculatedOutputPath->Size = System::Drawing::Size(371, 20);
			this->txtboxCalculatedOutputPath->TabIndex = 16;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 83);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(120, 13);
			this->label1->TabIndex = 15;
			this->label1->Text = L"Calculated Output Path:";
			// 
			// btnGetDutyDateArray
			// 
			this->btnGetDutyDateArray->Location = System::Drawing::Point(530, 38);
			this->btnGetDutyDateArray->Name = L"btnGetDutyDateArray";
			this->btnGetDutyDateArray->Size = System::Drawing::Size(146, 23);
			this->btnGetDutyDateArray->TabIndex = 14;
			this->btnGetDutyDateArray->Text = L"Get Lunch Duty Date List";
			this->btnGetDutyDateArray->UseVisualStyleBackColor = true;
			this->btnGetDutyDateArray->Click += gcnew System::EventHandler(this, &Form1::btnGetDutyDateArray_Click);
			// 
			// txtboxLunchDutyDaysList
			// 
			this->txtboxLunchDutyDaysList->Location = System::Drawing::Point(154, 40);
			this->txtboxLunchDutyDaysList->Name = L"txtboxLunchDutyDaysList";
			this->txtboxLunchDutyDaysList->Size = System::Drawing::Size(371, 20);
			this->txtboxLunchDutyDaysList->TabIndex = 13;
			// 
			// lblLunchDutyDaysList
			// 
			this->lblLunchDutyDaysList->AutoSize = true;
			this->lblLunchDutyDaysList->Location = System::Drawing::Point(10, 43);
			this->lblLunchDutyDaysList->Name = L"lblLunchDutyDaysList";
			this->lblLunchDutyDaysList->Size = System::Drawing::Size(139, 13);
			this->lblLunchDutyDaysList->TabIndex = 12;
			this->lblLunchDutyDaysList->Text = L"Lunch Duty Days List Path: ";
			// 
			// btnGetTeacherList
			// 
			this->btnGetTeacherList->Location = System::Drawing::Point(530, 15);
			this->btnGetTeacherList->Name = L"btnGetTeacherList";
			this->btnGetTeacherList->Size = System::Drawing::Size(146, 23);
			this->btnGetTeacherList->TabIndex = 11;
			this->btnGetTeacherList->Text = L"Get Teacher List";
			this->btnGetTeacherList->UseVisualStyleBackColor = true;
			this->btnGetTeacherList->Click += gcnew System::EventHandler(this, &Form1::btnGetTeacherList_Click);
			// 
			// txtboxTeacherListPath
			// 
			this->txtboxTeacherListPath->Location = System::Drawing::Point(154, 17);
			this->txtboxTeacherListPath->Name = L"txtboxTeacherListPath";
			this->txtboxTeacherListPath->Size = System::Drawing::Size(371, 20);
			this->txtboxTeacherListPath->TabIndex = 10;
			// 
			// lblTeacherListPath
			// 
			this->lblTeacherListPath->AutoSize = true;
			this->lblTeacherListPath->Location = System::Drawing::Point(10, 20);
			this->lblTeacherListPath->Name = L"lblTeacherListPath";
			this->lblTeacherListPath->Size = System::Drawing::Size(97, 13);
			this->lblTeacherListPath->TabIndex = 9;
			this->lblTeacherListPath->Text = L"Teacher List Path: ";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(692, 451);
			this->Controls->Add(this->btnCalculateTeacherDuties);
			this->Controls->Add(this->txtboxCalculatedOutputPath);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnGetDutyDateArray);
			this->Controls->Add(this->txtboxLunchDutyDaysList);
			this->Controls->Add(this->lblLunchDutyDaysList);
			this->Controls->Add(this->btnGetTeacherList);
			this->Controls->Add(this->txtboxTeacherListPath);
			this->Controls->Add(this->lblTeacherListPath);
			this->Name = L"Form1";
			this->Text = L"TeacherDuties";
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		// INPUT:    A CSV list of teachers' NAMES and LUNCH PERIODS
		// ACTIONS:  This data is input into an SQL database
		// OUTPUT:   None

		private: System::Void btnGetTeacherList_Click(System::Object^  sender, System::EventArgs^  e) {

			// DEBUG: Test other functions 


			 // Get specified file path
			 System::String^ filePath = this->txtboxTeacherListPath->Text;

			 // Check if specified file exists - if not, exit the function
			 System::Boolean fileExists = System::IO::File::Exists(this->txtboxTeacherListPath->Text);
			if (fileExists.Equals(false))
			{
				MessageBox::Show("Error! Teacher list path is invalid.");
				return;
			}

			// Misc values for upcoming series of loops
			char comma = ',';

			// Read each line of the file (as a string) and analyze it for data
			// (http://msdn.microsoft.com/en-us/library/system.io.streamreader.peek.aspx#Y460)
			StreamReader^ textStream = gcnew StreamReader( filePath );
			while (textStream->Peek() > -1) {
				
					// Split the text between each set of commas into an array of strings
					array<String^> ^ textSeparatedArray = textStream->ReadLine()->Split(comma);

					// Enter the data for each teacher into the database
					for each (System::String^ str in textSeparatedArray) {  // Reference: http://msdn.microsoft.com/en-us/library/ms177202%28v=VS.100%29.aspx

						// Check to make sure the string is not null - if so, skip it
						if (str->Length < 1)
						{
							continue;
						}

						// Remove any spaces from the string
						str = str->Replace(" ","");

						// Enter data contained in "str" into the proper database
						// These conceptual functions work like the list.add function in VB.net
						if ((str->ToLower()->Equals("first")) | (str->ToLower()->Equals("second")))
						{
							MessageBox::Show(String::Concat("Lunch ",str));

							// Add the teacher's name to the database
							//Database.Add(str, "TeacherName")
						}
						else
						{
							// Add the teacher's lunch period to the database
							MessageBox::Show(String::Concat("Teacher [",str,"]"));
							//Database.Add(str, "TeacherLunchPeriod")
						}

					}

			} 

			// Close the text stream - we don't need it anymore
			textStream->Close();

		}

		// INPUT: A CSV file containing duty dates
		// ACTIONS: Stores the input dates in the database in a per-week format
		// OUTPUT: None
		
		// NOTE: This assumes all weeks listed are 5-day intervals
		//		 It also assumes that there are no duplicates in the duty date list
		
		private: System::Void btnGetDutyDateArray_Click(System::Object^  sender, System::EventArgs^  e) {
	
			// Step 1: Get specified file path (for duty dates list)
			System::String^ filePath = this->txtboxLunchDutyDaysList->Text;

			// Step 2: Check if specified file exists - if not, exit the function
			System::Boolean fileExists = System::IO::File::Exists(filePath);
			if (fileExists.Equals(false))
			{
				MessageBox::Show("Error! Teacher duty dates list is invalid.");
				return;
			}

			// Step 3: Get line count using an instance of StreamReader
			StreamReader^ lineCntStream = gcnew StreamReader( filePath );
			int lineCnt = 0;
			while (lineCntStream->Peek() > -1)
			{
				lineCntStream->ReadLine();
				lineCnt++;
			}
			lineCntStream->Close();

			// Step 4: Add each duty date instance to an array using a new instance of StreamReader
			array< String^ > ^ dutyDateArray = gcnew array< String^ >( lineCnt );
			StreamReader^ textStream = gcnew StreamReader( filePath );
			int i = 0;							// Loop counter
			while (textStream->Peek() > -1)
			{
				// Add current line to dutyDateArray
				String^ readLine = textStream->ReadLine();
				dutyDateArray->SetValue(readLine, i);

				i++;
			}

			// [Mr. Smith] Step 5: Set the "DutyDate" column of the "DutyDateArray" database equal to the "dutyDateArray" array
			// 

			// Return
			return;
		}
	
		private: System::Void btnCalculateTeacherDuties_Click(System::Object^  sender, System::EventArgs^  e)
		{

			// Get output
			String^ filePath = this->txtboxCalculatedOutputPath->Text;
			BuildTeacherDuties::outputDataToFile(filePath);

		}
	};

	
}

#pragma endregion
	
