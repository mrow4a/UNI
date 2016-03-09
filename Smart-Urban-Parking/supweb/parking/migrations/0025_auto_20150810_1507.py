# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0024_auto_20150810_1444'),
    ]

    operations = [
        migrations.CreateModel(
            name='Lamp_Error',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('timestamp', models.DateTimeField(default=django.utils.timezone.now, blank=True)),
                ('error_status', models.CharField(default=b'None', max_length=256, db_column=b'error_status', blank=True)),
                ('lamp_location', models.ForeignKey(to='parking.Lamp', null=True)),
            ],
        ),
        migrations.RemoveField(
            model_name='lamp_errors',
            name='lamp_location',
        ),
        migrations.DeleteModel(
            name='Lamp_Errors',
        ),
    ]
