using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using DeepEqual.Syntax;

namespace A7
{
    [TestFixture]
    class PersonDictonaryTest
    {
        PersonDictionary personTest = new PersonDictionary();

        public bool AreSameList<T>(List<T> list1, List<T> list2)
        {
            if (list1.Count != list2.Count)
            {
                return false;
            }
            for(int i = 0; i < list1.Count - 1; i++)
            {
                if (!(list1[i].IsDeepEqual(list2[i]))) 
                {
                    return false;
                }
            }
            return true;
        }

        [Test]
        public void GetByFirstTest()
        {
            List<Person> Allen = new List<Person> { new Person("Al", "Allenton", 35) };
            List<Person> Bill = new List<Person> { new Person("Bill", "Billoughton", 27), new Person("Bill", "Broughton", 25) };
            List<Person> Chris = new List<Person> { new Person("Chris", "Christiansen", 21), new Person("Chris", "Christiansen", 29) };
            List<Person> Debora = new List<Person> { new Person("Debora", "Deanson", 44), new Person("Debora", "Deanson", 44) };

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetByFirst.txt");
            Assert.That(0, Is.EqualTo(personTest.GetByFirst("noname").Count));
            Assert.That(AreSameList(Allen, personTest.GetByFirst("Al")), Is.True);
            Assert.That(AreSameList(Bill, personTest.GetByFirst("Bill")),  Is.True);
            Assert.That(AreSameList(Chris, personTest.GetByFirst("Chris")), Is.True);
            Assert.That(AreSameList(Debora, personTest.GetByFirst("Debora")), Is.True);
        }

       [Test]
       public void GetByLastTest()
        {
            List<Person> Danielson = new List<Person> { new Person("Dean", "Danielson", 33) };
            List<Person> Errington = new List<Person> { new Person("Earl", "Errington", 56), new Person("Edith", "Errington", 54) };
            List<Person> Finkleton = new List<Person> { new Person("Frank", "Finkleton", 21), new Person("Frank", "Finkleton", 48) };
            List<Person> Gil = new List<Person> { new Person("Gary", "Gil", 24), new Person("Gary", "Gil", 24) };

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetByLast.txt");
            Assert.That(0, Is.EqualTo(personTest.GetByLast("noname").Count));
            Assert.That(AreSameList(Danielson, personTest.GetByLast("Danielson")), Is.True);
            Assert.That(AreSameList(Errington, personTest.GetByLast("Errington")), Is.True);
            Assert.That(AreSameList(Finkleton, personTest.GetByLast("Finkleton")), Is.True);
            Assert.That(AreSameList(Gil, personTest.GetByLast("Gil")), Is.True);
        }

        [Test]
        public void GetByAgeTest()
        {
            List<Person> Seventysix =  new List<Person> { new Person("Gertrude", "Gimbalson", 76) };
            List<Person> Sixteen = new List<Person> { new Person("Harry", "Howsen", 16), new Person("Howard", "Hillson", 16) };
            List<Person> Twentynine = new List<Person> { new Person("Isaac", "Ivingtin", 29), new Person("Isaac", "Ivington", 29) };
            List<Person> Fiftysix = new List<Person> { new Person("James", "Juarez", 56), new Person("James", "Juarez", 56) };

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetByAge.txt");
            Assert.That(0, Is.EqualTo(personTest.GetByAge(-1).Count));
            Assert.That(AreSameList(Seventysix, personTest.GetByAge(76)), Is.True);
            Assert.That(AreSameList(Sixteen, personTest.GetByAge(16)), Is.True);
            Assert.That(AreSameList(Twentynine, personTest.GetByAge(29)), Is.True);
            Assert.That(AreSameList(Fiftysix, personTest.GetByAge(56)), Is.True);
        }

        [Test]
        public void GetAgeTest()
        {
            List<int> Jane = new List<int> { 67 };
            List<int> Kris = new List<int> { 22, 66 };
            List<int> Lonnie = new List<int> { 43, 43 };

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetAge.txt");
            Assert.That(AreSameList(Jane, personTest.GetAge("Jane Jamison")), Is.True);
            Assert.That(AreSameList(Kris, personTest.GetAge("Kris Kristopherson")), Is.True);
            Assert.That(AreSameList(Lonnie, personTest.GetAge("Lonnie Lemson")), Is.True);
        }

        [Test]
        public void GetOldestTest()
        {
            personTest.Clear();
            Assert.That(0, Is.EqualTo(personTest.GetOldest().Count));

            List<Person> Onetwenty = new List<Person> { new Person("Larry", "Larson", 120) };
            List<Person> Onezeroone = new List<Person> { new Person("Mary", "Mishim", 101), new Person("Mike", "Mendoza", 101) };
            List<Person> Seventyfive = new List<Person> { new Person("Nick", "Nicholson", 75), new Person("Nick", "Nicholson", 75) };

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetOldest1.txt");
            Assert.That(AreSameList(Onetwenty, personTest.GetOldest()), Is.True);

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetOldest2.txt");
            Assert.That(AreSameList(Onezeroone, personTest.GetOldest()), Is.True);

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetOldest3.txt");
            Assert.That(AreSameList(Seventyfive, personTest.GetOldest()), Is.True);
        }

        [Test]
        public void GetYoungestTest()
        {
            personTest.Clear();
            Assert.That(0, Is.EqualTo(personTest.GetOldest().Count));

            List<Person> Two = new List<Person> { new Person("Orrin", "Orville", 2) };
            List<Person> One = new List<Person> { new Person("Perry", "Perkins", 1), new Person("Patricia", "Patton", 1) };
            List<Person> Three = new List<Person> { new Person("Quinn", "Quigley", 3), new Person("Quinn", "Quigley", 3) };

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetYoungest1.txt");
            Assert.That(AreSameList(Two, personTest.GetYoungest()), Is.True);

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetYoungest2.txt");
            Assert.That(AreSameList(One, personTest.GetYoungest()), Is.True);

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetYoungest3.txt");
            Assert.That(AreSameList(Three, personTest.GetYoungest()), Is.True);
        }

        [Test]
        public void StartIterationTest()
        {
            List<Person> Sorted = new List<Person> {
                new Person("Chris", "Christiansen", 21),
                new Person("Bill", "Broughton", 25),
                new Person("Bill", "Billoughton", 27),
                new Person("Chris", "Christiansen", 29),
                new Person("Al", "Allenton", 35),
                new Person("Debora", "Deanson", 44),
                new Person("Debora", "Deanson", 44)
            };

            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetByFirst.txt");
            personTest.StartIteration();
            Assert.That(AreSameList(Sorted, personTest.iteratorList), Is.True);
        }

        [Test]
        public void IsDoneTest()
        {
            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetByFirst.txt");
            personTest.StartIteration();
            Assert.That(personTest.IsDone(), Is.False);
            personTest.GetNext();
            Assert.That(personTest.IsDone(), Is.False);
            personTest.GetNext();

            for(int i = 0; i < personTest.iteratorList.Count - 3; i++)
            {
                personTest.GetNext();
            }

            Assert.That(personTest.IsDone(), Is.True);
        }

        [Test]
        public void GetNextTest()
        {
            personTest.ReadFile(TestContext.CurrentContext.TestDirectory + @"\GetNext.txt");
            personTest.StartIteration();

            Assert.That(new Person("Timmy", "Thompson", 24).IsDeepEqual(personTest.GetNext()), Is.True);
            Assert.That(new Person("Toby", "Thompson", 85).IsDeepEqual(personTest.GetNext()), Is.True);
            Assert.That(new Person("Toby", "Thompson", 87).IsDeepEqual(personTest.GetNext()), Is.True);

            Assert.Throws<System.ArgumentOutOfRangeException>(() => personTest.GetNext());
        }
    }
}
