using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace A7
{
    class PersonDictionary
    {
        public Dictionary<string, List<Person>> persons;
        public List<Person> iteratorList;
        public int iteratorIndex;

        public PersonDictionary()
        {
            persons = new Dictionary<string, List<Person>>();
            iteratorList = new List<Person>();
            iteratorIndex = 0;
        }

        public void ReadFile(string filename)
        {
            Clear();

            StreamReader fileReader = new StreamReader(filename);
            string personLine;
            
            while ((personLine = fileReader.ReadLine()) != null)
            {
                string[] personTokens = personLine.Split();
                PushPerson(new Person(personTokens[0], personTokens[1], int.Parse(personTokens[2])));
            }
        }

        public List<Person> GetByFirst(string firstName)
        {
            List<string> validKeys = new List<string>();
            foreach(string key in persons.Keys)
            {
                if(key.StartsWith(firstName))
                {
                    validKeys.Add(key);
                }
            }

            List<Person> personsList = new List<Person>();
            if (validKeys.Count > 0)
            {
                foreach(string key in validKeys)
                {
                    personsList.AddRange(persons[key]);
                }
            }

            return personsList;
        }

        public List<Person> GetByLast(string lastname)
        {
            List<string> validKeys = new List<string>();
            foreach (string key in persons.Keys)
            {
                if (key.EndsWith(lastname))
                {
                    validKeys.Add(key);
                }
            }

            List<Person> personsList = new List<Person>();
            if (validKeys.Count > 0)
            {
                foreach (string key in validKeys)
                {
                    personsList.AddRange(persons[key]);
                }
            }

            return personsList;
        }

        public List<Person> GetByAge(int age)
        {
            List<Person> personsList = new List<Person>();
            foreach(var item in persons)
            {
                foreach(Person p in item.Value)
                {
                    if (p.Age == age)
                    {
                        personsList.Add(p);
                    }
                }
            }
            return personsList;
        }

        public List<int> GetAge(string name)
        {
            List<int> ages = new List<int>();
            foreach(Person p in persons[name])
            {
                ages.Add(p.Age);
            }

            return ages;
        }

        public List<Person> GetOldest()
        {
            int oldest = 0;
            List<Person> oldestPersons = new List<Person>();

            foreach(var item in persons)
            {
                foreach (Person p in item.Value)
                {
                    if (p.Age > oldest)
                    {
                        oldest = p.Age;
                    }
                }
            }

            foreach(var item in persons)
            {
                foreach (Person p in item.Value)
                {
                    if(p.Age == oldest)
                    {
                        oldestPersons.Add(p);
                    }
                }
            }

            return oldestPersons;
        }

        public List<Person> GetYoungest()
        {
            int youngest = 1000;
            List<Person> youngestPersons = new List<Person>();

            foreach (var item in persons)
            {
                foreach (Person p in item.Value)
                {
                    if (p.Age < youngest)
                    {
                        youngest = p.Age;
                    }
                }
            }

            foreach (var item in persons)
            {
                foreach (Person p in item.Value)
                {
                    if (p.Age == youngest)
                    {
                        youngestPersons.Add(p);
                    }
                }
            }

            return youngestPersons;
        }

        public void StartIteration()
        {
            foreach(string key in persons.Keys)
            {
                iteratorList.AddRange(persons[key]);
            }

            iteratorList.Sort((x, y) => x.Age.CompareTo(y.Age));
        }

        public bool IsDone()
        {
            return iteratorIndex == iteratorList.Count - 1;
        }

        public Person GetNext()
        {
            Person p = iteratorList[iteratorIndex];
            iteratorIndex++;
            return p;
        }

        public void PushPerson(Person p)
        {
            List<Person> pList;

            if (!persons.TryGetValue(p.GetFullName(), out pList))
            {
                pList = new List<Person>();
                persons.Add(p.GetFullName(), pList);
            }

            pList.Add(p);
        }

        public void Clear()
        {
            persons.Clear();
            iteratorList.Clear();
            iteratorIndex = 0;
        }
    }
}
