#include <bits/stdc++.h>
using namespace std;

//vehicle class
class Vehicle{
    public:
        //vehicle Registration number
        string RegNo;

        //constructor
        Vehicle(string RegNo){
            this->RegNo=RegNo;
        }
};

//class of parking
class ParkingLot {
private:
    //bike and car count record
    int bike_cnt=0,car_cnt=0;
    int bike_maxsize=0,car_maxsize=0;

public:
    //list of bike and cars slots
    vector<int> CarSlots,BikeSlots;
    //car slot with bike
    unordered_map<int,int> carTOBikePark;

    //Creating map which mapping key with value
    unordered_map<int,string> slotToRegNoBike,slotToRegNoCar;
    unordered_map<string,int> CarToSlot,BikeToSlot,RegNoToSlot;
    unordered_map<string,int> VehicleToEntryTime,VehicleToExitTime;
    unordered_map<string,int> VehicleToAmount;


    //Rate of parking car in Hours
    int carRateCard(int entry_time,int exit_time){
        //calculating cost a/c to time
        int cost;
        int time=exit_time-entry_time;
        if(time<=1)
            cost=20;
        else if(time>1 && time<=3)
            cost=50;
        else if(time>3 && time<=24)
            cost=80;
        else{
            cost=(time/24)*100;
        }
        return cost;
    }


    //Rate of parking Bike in Hours
    int bikeRateCard(int entry_time,int exit_time){
        //calculating cost a/c to time
        int cost;
        int time=exit_time-entry_time;
        if(time<=1)
            cost=10;
        else if(time>1 && time<=3)
            cost=20;
        else if(time>3 && time<=24)
            cost=50;
        else{
            cost=(time/24)*70;
        }
        return cost;
    }


    //checking slot is full or not
    bool isFull(int current_vehicle_cnt,int max_size){
        if(current_vehicle_cnt==max_size)
            return true;
        else
            return false;
    }


    //checking slot is empty or not
    bool isEmpty(int current_vehicle_cnt,int max_size){
        if(current_vehicle_cnt==0 and max_size!=0)
            return true;
        else
            return false;
    }
    

    //registering a parking lot
    void createParkingLot(int carCount,int bikeCount) {
        //checking bike count
        try{
            if(bikeCount<0)
                throw bikeCount;
            this->bike_maxsize = bikeCount;
        } 
        catch(int lot){
            cout<<"Invalid Bike lot count"<<endl;
        }

        //checking car count
        try{
            if(carCount<0)
                throw carCount;
            this->car_maxsize = carCount;
        } 
        catch(int lot){
            cout<<"Invalid Car lot count"<<endl;
        }

        //Initially assigning slots of bike and car to 0 (means empty)
        CarSlots.assign(car_maxsize+1,0);
        BikeSlots.assign(bike_maxsize+1,0);
        cout<<"Created parking lot with "<<carCount<<" Car slots and "<<bikeCount<<" Bike slots\n";
    }


    //Parking a vehicle
    void Park(string RegNo,string VehicleType,int EntryTime) {
        //If vehicle is car
        if(VehicleType=="Car"){
            //checking car lot is created or not
            if(this->car_maxsize==0){
                cout<<"Sorry, Car parking lot is not created"<<endl;
            }

            //checking car lot is full or not
            else if(isFull(this->car_cnt,this->car_maxsize)){
                cout<<"Sorry, Car parking lot is full"<<endl;
            } 
            
            //else find vacant slot assign to 1(means occupied)
            else{
                int vacant;
                //1-based indexing as created list above 1-based.
                for(int idx=1;idx<=car_maxsize;idx++){
                    //checking if 0 (means vacant)
                    if(CarSlots[idx]==0){
                        //finding vacant position
                        vacant=idx;
                        break;
                    }
                }

                //filling slot by changing value from 0 to 1.
                CarSlots[vacant]=1;

                //car to slot mapping-> mp[regNO]=slotNo.
                CarToSlot[RegNo]=vacant;

                //slot to RegNo mapping-> mp[slot]=regNO.
                slotToRegNoCar[vacant] = RegNo;

                //RegNo to EntryTime mapping-> mp[RegNo]=EntryTime
                VehicleToEntryTime[RegNo]=EntryTime;

                //RegNO to Slot mapping
                RegNoToSlot[RegNo]=vacant;
                
                //car to bike parking mapping
                carTOBikePark[vacant]=2;

                //incrementing car cnt.
                car_cnt++;
                cout<<"Car with vehicle registration number \""<<RegNo<<"\" has been parked at car slot number "<<vacant<<"\n";
            }
        }

        //If vehicle is bike
        else if(VehicleType=="Bike"){
            //checking bike lot is created or not
            if(this->bike_maxsize==0){
                cout<<"Sorry, Bike parking lot is not created"<<endl;
            } 

            //checking bike lot is full or not
            else if(isFull(this->bike_cnt,this->bike_maxsize)){
                //now check car lot is full to not to park 2 bikes
                if(isFull(this->car_cnt,this->car_maxsize) || this->car_maxsize==0)
                    cout<<"Sorry, Bike parking lot is full"<<endl;
                else{
                    int vacant;
                    bool checkCarSlotWithBike=false;
                    //1-based indexing as created list above 1-based
                    for(int idx=1;idx<=car_maxsize;idx++){
                        if(carTOBikePark[idx]==1){
                            checkCarSlotWithBike=true;
                            vacant=idx;
                            break;
                        }
                        //checking if 0 (means vacant)
                        else if(CarSlots[idx]==0){
                            //finding vacant position
                            vacant=idx;
                            break;
                        }
                    }
                    if(checkCarSlotWithBike){
                        carTOBikePark[vacant]++;
                        
                        //Bike to slot mapping-> mp[regNO]=slotNo.
                        BikeToSlot[RegNo]=vacant;
        
                        //slot to RegNo mapping-> mp[slot]=regNO.
                        //slotToRegNoBike[vacant] = RegNo;
        
                        //RegNo to EntryTime mapping-> mp[RegNo]=EntryTime
                        VehicleToEntryTime[RegNo]=EntryTime;
        
                        //RegNO to Slot mapping
                        RegNoToSlot[RegNo]=vacant;
        
                        //incrementing bike cnt.
                        bike_cnt++;
                        cout<<"Bike with regno. "<<RegNo<<" is parked on car slot due to full bike slots\n";
                    }
                    else{
                        //filling slot by changing value from 0 to 1.
                        CarSlots[vacant]=1;
                        
                        //checking if car lot is already parked 2 bikes
                        if(carTOBikePark[vacant]==2){
                            cout<<"Sorry, Bike parking lot is full\n";
                        }
                        //else park the bike and increment car lot space by 1
                        else{
                            carTOBikePark[vacant]++;
                            //increment car count because the bike takes space of car lot.
                            car_cnt++;
                            
                            //Bike to slot mapping-> mp[regNO]=slotNo.
                            BikeToSlot[RegNo]=vacant;
            
                            //slot to RegNo mapping-> mp[slot]=regNO.
                            //slotToRegNoBike[vacant] = RegNo;
            
                            //RegNo to EntryTime mapping-> mp[RegNo]=EntryTime
                            VehicleToEntryTime[RegNo]=EntryTime;
            
                            //RegNO to Slot mapping
                            RegNoToSlot[RegNo]=vacant;
            
                            //incrementing bike cnt.
                            bike_cnt++;
                            cout<<"Bike with regno. "<<RegNo<<" is parked on car slot due to full bike slots\n";
                        } 
                    }
                    
                }
            } 

            //else find vacant slot assign to 1(means occupied)
            else{
                int vacant;
                //1-based indexing as created list above 1-based.
                for(int idx=1;idx<=bike_maxsize;idx++){
                    //checking if 0 means vacant
                    if(BikeSlots[idx]==0){
                        vacant=idx;
                        break;
                    }
                }

                //filling slot by changing value from 0 to 1.
                BikeSlots[vacant]=1;

                //Bike to slot mapping-> mp[regNO]=slotNo.
                BikeToSlot[RegNo]=vacant;

                //slot to RegNo mapping-> mp[slot]=regNO.
                slotToRegNoBike[vacant] = RegNo;

                //RegNo to EntryTime mapping-> mp[RegNo]=EntryTime
                VehicleToEntryTime[RegNo]=EntryTime;

                //RegNO to Slot mapping
                RegNoToSlot[RegNo]=vacant;

                //incrementing car cnt.
                bike_cnt++;
                cout<<"Bike with vehicle registration number \""<<RegNo<<"\" has been parked at Bike slot number "<<vacant<<"\n";
            }
        }
    }


    //Exiting a vehicle from parking lot
    void ExitVehicle(int SlotNo,string VehicleType,int ExitTime){
        //if car is leaving
        if(VehicleType=="Car"){
            //checking car lot is created or not
            if(this->car_maxsize==0){
                cout<<"Sorry, Car parking lot is not created"<<endl;
            }

            //checking car lot is empty or not
            else if(isEmpty(this->car_cnt,this->car_maxsize)){
                cout<<"Car Parking lot is empty"<<endl;
            }

            //else empty occupied slot by changing 1 to 0
            else if(CarSlots[SlotNo]==1){
                CarSlots[SlotNo]=0;

                //entry time of car
                int EntryTime=VehicleToEntryTime[slotToRegNoCar[SlotNo]];

                //storing exit time of car
                VehicleToExitTime[slotToRegNoCar[SlotNo]]=ExitTime;

                //calculating parking amount
                int parkingAmount=carRateCard(EntryTime,ExitTime);

                //storing parking amt wrt to regno
                VehicleToAmount[slotToRegNoCar[SlotNo]]=parkingAmount;

                //VehicleToEntryTime.erase(slotToRegNoCar[SlotNo]);
                string RegNo=slotToRegNoCar[SlotNo];

                //delete the record of vehicle leaved
                slotToRegNoCar.erase(SlotNo);
                CarToSlot.erase(RegNo);

                //decrementing vehicle count of leaved vehicle
                car_cnt--;
                cout<<"Slot number "<<SlotNo<<" vacated, the car with vehicle registration number \""<<RegNo<<"\" left the space\n";
                cout<<"Parking Amount: "<<parkingAmount<<"\n";
            }
            else{
                cout<<"Slot number "<<SlotNo<<" of Car is already empty\n";
            }
        }

        else if(VehicleType=="Bike"){
            //checking bike lot is created or not
            if(this->bike_maxsize==0){
                cout<<"Sorry, Bike parking lot is not created"<<endl;
            }

            //checking car lot is empty or not
            else if(isEmpty(this->bike_cnt,this->bike_maxsize)){
                cout<<"Bike Parking lot is empty\n";
            }

            //else empty occupied slot by changing 1 to 0
            else if(BikeSlots[SlotNo]==1){
                BikeSlots[SlotNo]=0;

                //entry time of bike
                int EntryTime=VehicleToEntryTime[slotToRegNoBike[SlotNo]];

                //storing exit time of bike
                VehicleToExitTime[slotToRegNoBike[SlotNo]]=ExitTime;

                //calculating parking amount
                int parkingAmount=bikeRateCard(EntryTime,ExitTime);

                //storing parking amt wrt to regno
                VehicleToAmount[slotToRegNoBike[SlotNo]]=parkingAmount;

                //VehicleToEntryTime.erase(slotToRegNoBike[SlotNo]);
                string RegNo=slotToRegNoBike[SlotNo];

                //delete the record of vehicle leaved
                slotToRegNoBike.erase(SlotNo);
                BikeToSlot.erase(RegNo);

                //decrementing vehicle count of leaved vehicle
                bike_cnt--;
                cout<<"Slot number "<<SlotNo<<" vacated, the Bike with vehicle registration number \""<<RegNo<<"\" left the space\n";
                cout<<"Parking Amount: "<<parkingAmount<<"\n";
            }
            else{
                cout<<"Slot number "<<SlotNo<<" of Bike is already empty\n";
            }
        } 
    }


    //parking history details
    void parkingHistory(string RegNo){
        cout<<"Vehicle Details:\n";

        //first check vehicle is present in lot or not by regno
        if(RegNoToSlot.find(RegNo)!=RegNoToSlot.end()){
            cout<<"Vehicle Parking slot: "<<RegNoToSlot[RegNo]<<"\n";

            //check if vehicle leave the parking lot or not
            if(VehicleToExitTime.find(RegNo)!=VehicleToExitTime.end()){
                int duration=VehicleToExitTime[RegNo]-VehicleToEntryTime[RegNo];
                cout<<"Duration of vehicle in Parking lot: "<<duration<<"HR\n";
                cout<<"Amount paid by vehicle: Rs."<<VehicleToAmount[RegNo]<<"\n";
            }
            else{
                cout<<"Vehicle still in Parking lot\n";
            }
        }
        else{
            cout<<"Vehicle Not Registered to parking lot yet\n";
        }
    }
};



int main() {
    //code
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);

    //Take command as string for managing parking lot.
    string command;

    //creating an object of ParkingLot.
    ParkingLot parkingLot;

    //now taking commands.
    while(cin>>command){
        //Registering parking lot 
        if(command=="Create_ParkingLot"){
            int carCount,bikeCount;
            //cout<<"Enter car count\n";
            cin>>carCount;
            //cout<<"Enter bike count\n";
            cin>>bikeCount;
            parkingLot.createParkingLot(carCount,bikeCount);
        }


        //parking a Vehicle
        else if(command=="Park"){
            string RegNo;
            cin>>RegNo;
            string VehicleType;
            //cout<<"Enter Vehicle Type"<<"\n";
            cin>>VehicleType;   //Bike or Car.
            //time should be in 24-HR Based 0-means 12am and 22-10pm
            int EntryTime;
            //cout<<"Enter time of arrival\n";
            cin>>EntryTime;
            parkingLot.Park(RegNo,VehicleType,EntryTime);
        }


        //Exiting a Vehicle from parking lot.
        else if(command=="Exit_Vehicle"){
            int SlotNo;
            //cout<<"Enter Slot "
            cin>>SlotNo;
            string VehicleType;
            //cout<<"Enter Vehicle Type"<<"\n";
            cin>>VehicleType;   //Bike or Car.
            int ExitTime;
            //cout<<"Enter time of arrival\n";
            cin>>ExitTime;
            parkingLot.ExitVehicle(SlotNo,VehicleType,ExitTime);
        }


        //parking History
        else if(command=="Parking_History"){
            //Vehical RegNO.
            string RegNo;
            cin>>RegNo;
            parkingLot.parkingHistory(RegNo);
        }
    }
    return 0;
}
